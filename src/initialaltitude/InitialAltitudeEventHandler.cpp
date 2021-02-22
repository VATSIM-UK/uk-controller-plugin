#include "pch/stdafx.h"
#include "initialaltitude/InitialAltitudeEventHandler.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "ownership/AirfieldOwnershipManager.h"
#include "controller/ActiveCallsignCollection.h"
#include "initialaltitude/InitialAltitudeGenerator.h"
#include "timedevent/DeferredEventHandler.h"
#include "login/Login.h"
#include "flightplan/DeferredFlightplanEvent.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "euroscope/GeneralSettingsEntries.h"

using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::InitialAltitude::InitialAltitudeGenerator;
using UKControllerPlugin::Ownership::AirfieldOwnershipManager;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Airfield::NormaliseSid;
using UKControllerPlugin::TimedEvent::DeferredEventHandler;
using UKControllerPlugin::Controller::Login;
using UKControllerPlugin::Flightplan::DeferredFlightPlanEvent;
using UKControllerPlugin::Flightplan::StoredFlightplanCollection;
using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;
using UKControllerPlugin::Euroscope::GeneralSettingsEntries;

namespace UKControllerPlugin {
    namespace InitialAltitude {

        InitialAltitudeEventHandler::InitialAltitudeEventHandler(
            const InitialAltitudeGenerator & generator,
            const ActiveCallsignCollection & activeCallsigns,
            const AirfieldOwnershipManager & airfieldOwnership,
            const Login & login,
            DeferredEventHandler & deferredEvents,
            EuroscopePluginLoopbackInterface & plugin,
            const StoredFlightplanCollection& storedFlightplans
        )
            : generator(generator), activeCallsigns(activeCallsigns), airfieldOwnership(airfieldOwnership),
            minimumLoginTimeBeforeAssignment(5), login(login), deferredEvents(deferredEvents), plugin(plugin),
            storedFlightplans(storedFlightplans)
        {

        }

        /*
            Handle events regarding when a flightplan changes.
        */
        void InitialAltitudeEventHandler::FlightPlanEvent(
            EuroScopeCFlightPlanInterface & flightPlan,
            EuroScopeCRadarTargetInterface & radarTarget
        ) {
            if (!this->userAutomaticAssignmentsAllowed) {
                return;
            }

            // If we've not been logged in for long, wait a bit
            if (this->login.GetSecondsLoggedIn() < this->minimumLoginTimeBeforeAssignment) {
                LogDebug(
                    "Deferring initial altitude assignment for " + flightPlan.GetCallsign()
                );
                this->deferredEvents.DeferFor(
                    std::make_unique<DeferredFlightPlanEvent>(*this, this->plugin, flightPlan.GetCallsign()),
                    this->minimumLoginTimeBeforeAssignment
                );
                return;
            }

            // The aircraft has already had it's initial altitude set once, so don't double set
            // it for the same SID
            if (
                this->alreadySetMap.count(flightPlan.GetCallsign()) &&
                this->alreadySetMap.at(flightPlan.GetCallsign()) == flightPlan.GetSidName()
            ) {
                return;
            }


            // Check necessary conditions for assignment are met.
            if (!this->MeetsAssignmentConditions(flightPlan, radarTarget)) {
                return;
            }

            // Remove deprecation marks from the SID name.
            std::string sidName = normalise.StripSidDeprecation(flightPlan.GetSidName());

            // Make sure the SID exists.
            if (!generator.HasSid(flightPlan.GetOrigin(), sidName)) {
                return;
            }

            // Doesn't assign an init altitude if cruise level is less than the initial altitude of the SID.
            const int initialAltitude = generator.GetInitialAltitudeForDeparture(flightPlan.GetOrigin(), sidName);
            if (flightPlan.GetCruiseLevel() < initialAltitude) {
                return;
            }

            // Set the IA and record it
            LogInfo(
                "Set initial altitude for " + flightPlan.GetCallsign() +
                " (" + flightPlan.GetOrigin() + ", " + sidName + ")"
            );
            flightPlan.SetClearedAltitude(initialAltitude);
            this->alreadySetMap[flightPlan.GetCallsign()] = sidName;
        }

        InitialAltitudeEventHandler::~InitialAltitudeEventHandler(void)
        {
        }

        /*
            Handle events regarding when a flightplan disconnects.
        */
        void InitialAltitudeEventHandler::FlightPlanDisconnectEvent(EuroScopeCFlightPlanInterface & flightPlan)
        {
            this->alreadySetMap.erase(flightPlan.GetCallsign());
        }

        /*
            Handle events regarding controller data being changed (directs etc).
        */
        void InitialAltitudeEventHandler::ControllerFlightPlanDataEvent(
            EuroScopeCFlightPlanInterface & flightPlan,
            int dataType
        ) {
            // Nothing to do here
        }

        /*
            Reset the initial altitude for the given callsign.
        */
        void InitialAltitudeEventHandler::RecycleInitialAltitude(
            EuroScopeCFlightPlanInterface & flightplan,
            EuroScopeCRadarTargetInterface & radarTarget,
            std::string context,
            const POINT & mousePos
        ) {
            if (!MeetsForceAssignmentConditions(flightplan, radarTarget))
            {
                return;
            }

            std::string sidName = normalise.StripSidDeprecation(flightplan.GetSidName());

            if (!generator.HasSid(flightplan.GetOrigin(), sidName)) {
                return;
            }
            flightplan.SetClearedAltitude(
                this->generator.GetInitialAltitudeForDeparture(flightplan.GetOrigin(), sidName)
            );
            LogInfo(
                "Recycled initial altitude for " + flightplan.GetCallsign() +
                    " (" + flightplan.GetOrigin() + ", " + sidName + ")"
            );
            this->alreadySetMap[flightplan.GetCallsign()] = sidName;
        }

        /*
            Returns whether or not the user has allowed automatic assignments.
        */
        bool InitialAltitudeEventHandler::UserAutomaticAssignmentsAllowed(void) const
        {
            return this->userAutomaticAssignmentsAllowed;
        }

        /*
            Returns true if the aircraft meets the prerequisites for initial altitude assignment.

            To return true, must:

            1. Must not have a current flight level or distance from origin of exactly zero.
               This is required because for some reason EuroScope picks up flightplan changes over the likes of
               Austria and decides its time to do an initial altitude assignment.
            2. Be on the ground
            3. Be within a specified distance of the origin airport.
            4. Be under a certain speed.
            5. Not have a cleared altitude set.
            6. Not be tracked by any controller.
            7. Not be "simulated" by ES.
            8. Have a user that has a recognised callsign.
            9. Have the airfield of origin owned by the controller.
        */
        bool InitialAltitudeEventHandler::MeetsAssignmentConditions(
            EuroScopeCFlightPlanInterface & flightPlan,
            EuroScopeCRadarTargetInterface & radarTarget
        ) const {
            return radarTarget.GetFlightLevel() != 0 &&
                flightPlan.GetDistanceFromOrigin() != 0.0 &&
                radarTarget.GetFlightLevel() <= this->assignmentMaxAltitude &&
                flightPlan.GetDistanceFromOrigin() <= this->assignmentMaxDistanceFromOrigin &&
                radarTarget.GetGroundSpeed() <= this->assignmentMaxSpeed &&
                !flightPlan.HasControllerClearedAltitude() &&
                !flightPlan.IsTracked() &&
                !flightPlan.IsSimulated() &&
                this->airfieldOwnership.AirfieldOwnedByUser(flightPlan.GetOrigin());
        }

        /*
         * Force assignments are allowed if the aircraft is:
         *
         * 1. Not tracked
         * 2. Tracked by the current user
         */
        bool InitialAltitudeEventHandler::MeetsForceAssignmentConditions(
            EuroScopeCFlightPlanInterface& flightplan,
            EuroScopeCRadarTargetInterface& radarTarget
        ) {
            return !flightplan.IsTracked() || flightplan.IsTrackedByUser();
        }

        /*
            If its the user, do some updates
        */
        void InitialAltitudeEventHandler::ActiveCallsignAdded(const ActiveCallsign& callsign, bool userCallsign)
        {
            if (!userCallsign) {
                return;
            }


            LogInfo("Mass assigning initial altitudes");

            std::shared_ptr<EuroScopeCFlightPlanInterface> fp;
            std::shared_ptr<EuroScopeCRadarTargetInterface> rt;
            for (
                StoredFlightplanCollection::const_iterator it = this->storedFlightplans.cbegin();
                it != this->storedFlightplans.cend();
                ++it
            ) {
                fp = this->plugin.GetFlightplanForCallsign(it->second->GetCallsign());
                rt = this->plugin.GetRadarTargetForCallsign(it->second->GetCallsign());

                if (!fp || !rt) {
                    continue;
                }

                this->FlightPlanEvent(*fp, *rt);
            }
        }

        /*
            Nothing to see here
        */
        void InitialAltitudeEventHandler::ActiveCallsignRemoved(const ActiveCallsign& callsign, bool userCallsign)
        {

        }

        /*
            Nothing to see here
        */
        void InitialAltitudeEventHandler::CallsignsFlushed(void)
        {

        }

        /*
            Called when user settings get updated.
        */
        void InitialAltitudeEventHandler::UserSettingsUpdated(
            UKControllerPlugin::Euroscope::UserSetting & userSettings
        )
        {
            this->userAutomaticAssignmentsAllowed = userSettings.GetBooleanEntry(
                GeneralSettingsEntries::initialAltitudeToggleSettingsKey,
                true
            );
        }
    }  // namespace InitialAltitude
}  // namespace UKControllerPlugin
