#include "pch/stdafx.h"
#include "initialaltitude/InitialAltitudeEventHandler.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "airfield/AirfieldOwnershipManager.h"
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
using UKControllerPlugin::Airfield::AirfieldOwnershipManager;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Airfield::NormaliseSid;
using UKControllerPlugin::TimedEvent::DeferredEventHandler;
using UKControllerPlugin::Controller::Login;
using UKControllerPlugin::Flightplan::DeferredFlightPlanEvent;
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
            EuroscopePluginLoopbackInterface & plugin
        )
            : generator(generator), activeCallsigns(activeCallsigns), airfieldOwnership(airfieldOwnership),
            minimumLoginTimeBeforeAssignment(5), login(login), deferredEvents(deferredEvents), plugin(plugin)
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

			// Doesn't assign an IA if cruise level is less than the initial altitude of the SID.
			if (flightPlan.GetCruiseLevel() < generator.GetInitialAltitudeForDeparture(flightPlan.GetOrigin(), sidName)) {
				return; 
			}

            // Set the IA.
            LogInfo(
                "Set initial altitude for " + flightPlan.GetCallsign() +
                " (" + flightPlan.GetOrigin() + ", " + flightPlan.GetSidName() + ")"
            );
            flightPlan.SetClearedAltitude(
                this->generator.GetInitialAltitudeForDeparture(flightPlan.GetOrigin(), sidName)
            );
        }

        InitialAltitudeEventHandler::~InitialAltitudeEventHandler(void)
        {
        }

        /*
            Handle events regarding when a flightplan disconnects.
        */
        void InitialAltitudeEventHandler::FlightPlanDisconnectEvent(EuroScopeCFlightPlanInterface & flightPlan)
        {
            // Nothing to do here.
        }

        /*
            Handle events regarding controller data being changed (directs etc).
        */
        void InitialAltitudeEventHandler::ControllerFlightPlanDataEvent(
            EuroScopeCFlightPlanInterface & flightPlan,
            int dataType
        ) {
            // Nothing to do here.
        }

        /*
            Reset the initial altitude for the given callsign.
        */
        void InitialAltitudeEventHandler::RecycleInitialAltitude(
            EuroScopeCFlightPlanInterface & flightplan,
            EuroScopeCRadarTargetInterface & radarTarget
        ) {
            std::string sidName = normalise.StripSidDeprecation(flightplan.GetSidName());

            if (!generator.HasSid(flightplan.GetOrigin(), sidName)) {
                return;
            }
            flightplan.SetClearedAltitude(
                this->generator.GetInitialAltitudeForDeparture(flightplan.GetOrigin(), sidName)
            );
            LogInfo(
                "Recycled initial altitude for " + flightplan.GetCallsign() +
                    " (" + flightplan.GetOrigin() + ", " + flightplan.GetSidName() + ")"
            );
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

            1. Be within a specified distance of the origin airport.
            2. Be under a certain speed.
            3. Not have a cleared altitude set.
            4. Not be tracked by any controller.
            5. Not be "simulated" by ES.
            6. Have a user that has a recognised callsign.
            7. Have the airfield of origin owned by the controller.
        */
        bool InitialAltitudeEventHandler::MeetsAssignmentConditions(
            EuroScopeCFlightPlanInterface & flightPlan,
            EuroScopeCRadarTargetInterface & radarTarget
        ) {
            if (
                flightPlan.GetDistanceFromOrigin() > this->assignmentMaxDistanceFromOrigin ||
                radarTarget.GetGroundSpeed() > this->assignmentMaxSpeed ||
                flightPlan.HasControllerClearedAltitude() ||
                flightPlan.IsTracked() ||
                flightPlan.IsSimulated() ||
                !this->airfieldOwnership.AirfieldOwnedByUser(flightPlan.GetOrigin())
            ) {
                return false;
            }

            return true;
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
