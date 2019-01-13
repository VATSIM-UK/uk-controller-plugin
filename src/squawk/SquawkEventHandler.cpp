#include "pch/stdafx.h"
#include "squawk/SquawkEventHandler.h"
#include "controller/ControllerPosition.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "flightplan/StoredFlightplanCollection.h"
#include "flightplan/StoredFlightplan.h"
#include "controller/ActiveCallsignCollection.h"
#include "squawk/SquawkGenerator.h"
#include "login/Login.h"
#include "timedevent/DeferredEventHandler.h"
#include "flightplan/DeferredFlightplanEvent.h"

using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Squawk::SquawkGenerator;
using UKControllerPlugin::Flightplan::StoredFlightplanCollection;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Squawk::SquawkAssignment;
using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;
using UKControllerPlugin::Controller::Login;
using UKControllerPlugin::TimedEvent::DeferredEventHandler;
using UKControllerPlugin::Flightplan::DeferredFlightPlanEvent;

namespace UKControllerPlugin {
    namespace Squawk {

        SquawkEventHandler::SquawkEventHandler(
            SquawkGenerator & generator,
            const ActiveCallsignCollection & activeCallsigns,
            const StoredFlightplanCollection & storedFlightplans,
            EuroscopePluginLoopbackInterface & pluginLoopback,
            const Login & login,
            DeferredEventHandler & deferredEvents,
            bool automaticAssignmentDisabled
        )
            : generator(generator), activeCallsigns(activeCallsigns), storedFlightplans(storedFlightplans),
            pluginLoopback(pluginLoopback), login(login), deferredEvents(deferredEvents),
            automaticAssignmentDisabled(automaticAssignmentDisabled), minAutomaticAssignmentLoginTime(15)
        {

        }

        SquawkEventHandler::~SquawkEventHandler()
        {

        }

        /*
            Handle flightplan events
        */
        void SquawkEventHandler::FlightPlanEvent(
            EuroScopeCFlightPlanInterface & flightplan,
            EuroScopeCRadarTargetInterface & radarTarget
        ) {

            if (this->automaticAssignmentDisabled) {
                return;
            }

            // If we haven't been logged in more than a certain time, defer squawk assignments
            // to give EuroScope a chance to process controllers logged in / preexisting squawk assignments
            // We defer by the wait time to space out the subsequent events.
            if (this->login.GetSecondsLoggedIn() < this->minAutomaticAssignmentLoginTime) {
                LogInfo(
                    "Deferring squawk assignment for " + flightplan.GetCallsign() +
                    " as only recently logged in"
                );
                this->deferredEvents.DeferFor(
                    std::make_unique<DeferredFlightPlanEvent>(*this, this->pluginLoopback, flightplan.GetCallsign()),
                    this->minAutomaticAssignmentLoginTime
                );
                return;
            }

            this->generator.ReassignPreviousSquawkToAircraft(flightplan, radarTarget) ||
                this->generator.RequestLocalSquawkForAircraft(flightplan, radarTarget) ||
                this->generator.RequestGeneralSquawkForAircraft(flightplan, radarTarget);
        }

        /*
            Nothing to handle here
        */
        void SquawkEventHandler::FlightPlanDisconnectEvent(EuroScopeCFlightPlanInterface & flightPlan)
        {

        }

        /*
            If we get a controller assigned data update, make sure the residual squawk is set.
        */
        void SquawkEventHandler::ControllerFlightPlanDataEvent(EuroScopeCFlightPlanInterface & flightPlan, int dataType)
        {
            if (dataType != EuroScopePlugIn::CTR_DATA_TYPE_SQUAWK) {
                return;
            }

            if (!this->storedFlightplans.HasFlightplanForCallsign(flightPlan.GetCallsign())) {
                return;
            }

            this->storedFlightplans.GetFlightplanForCallsign(flightPlan.GetCallsign())
                .SetPreviouslyAssignedSquawk(flightPlan.GetAssignedSquawk());
        }

        /*
            Force a squawk recycle for a general squawk.
        */
        void SquawkEventHandler::SquawkReycleGeneral(
            EuroScopeCFlightPlanInterface & flightplan,
            EuroScopeCRadarTargetInterface & radarTarget
        ) const {
            LogInfo("Forcing general squawk update for " + flightplan.GetCallsign());
            this->generator.ForceGeneralSquawkForAircraft(flightplan, radarTarget);
        }

        /*
            Force a squawk recycle for a local squawk.
        */
        void SquawkEventHandler::SquawkRecycleLocal(
            EuroScopeCFlightPlanInterface & flightplan,
            EuroScopeCRadarTargetInterface & radarTarget
        ) const {
            LogInfo("Forcing local squawk update for " + flightplan.GetCallsign());
            this->generator.ForceLocalSquawkForAircraft(flightplan, radarTarget);
        }

        /*
            When the timed event goes off, check for tracked aircraft and whether they need squawks to be assigned.
            This is required because EuroScope doesn't provide a method to us akin to "OnAssumeAircraft".
        */
        void SquawkEventHandler::TimedEventTrigger(void)
        {
            if (!this->activeCallsigns.UserHasCallsign() || this->automaticAssignmentDisabled) {
                return;
            }

            for (
                StoredFlightplanCollection::const_iterator it = this->storedFlightplans.cbegin();
                it != this->storedFlightplans.cend();
                ++it
            ) {
                try {
                    if (this->pluginLoopback.GetFlightplanForCallsign(it->second->GetCallsign())->HasAssignedSquawk() ||
                        !this->pluginLoopback.GetFlightplanForCallsign(it->second->GetCallsign())->IsTrackedByUser()
                    ) {
                        continue;
                    }

                    if (!this->generator.RequestLocalSquawkForAircraft(
                        *this->pluginLoopback.GetFlightplanForCallsign(it->second->GetCallsign()),
                        *this->pluginLoopback.GetRadarTargetForCallsign(it->second->GetCallsign())
                    )) {
                        this->generator.RequestGeneralSquawkForAircraft(
                            *this->pluginLoopback.GetFlightplanForCallsign(it->second->GetCallsign()),
                            *this->pluginLoopback.GetRadarTargetForCallsign(it->second->GetCallsign()));
                    }
                }
                catch (std::invalid_argument) {
                    continue;
                }
            }
        }
    }  // namespace Squawk
}  // namespace UKControllerPlugin
