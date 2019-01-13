#pragma once
#include "timedevent/AbstractTimedEvent.h"
#include "flightplan/FlightPlanEventHandlerInterface.h"

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroScopeCFlightPlanInterface;
        class EuroScopeCRadarTargetInterface;
        class EuroscopePluginLoopbackInterface;
    }  // namespace Euroscope
    namespace Controller {
        class ActiveCallsignCollection;
        class Login;
    }  // namespace Controller
    namespace Flightplan {
        class StoredFlightplan;
        class StoredFlightplanCollection;
    }  // namespace Flightplan
    namespace Airfield {
        class AirfieldOwnershipManager;
    }  // namespace Airfield
    namespace Squawk {
        class SquawkGenerator;
    }  // namespace Squawk
    namespace TimedEvent {
        class DeferredEventHandler;
    }  // namespace TimedEvent
}  // namespace UKControllerPlugin


namespace UKControllerPlugin {
    namespace Squawk {

        class SquawkEventHandler : public UKControllerPlugin::Flightplan::FlightPlanEventHandlerInterface,
            public UKControllerPlugin::TimedEvent::AbstractTimedEvent
        {
            public:
                SquawkEventHandler(
                    UKControllerPlugin::Squawk::SquawkGenerator & generator,
                    const UKControllerPlugin::Controller::ActiveCallsignCollection & activeCallsigns,
                    const UKControllerPlugin::Flightplan::StoredFlightplanCollection & storedFlightplans,
                    UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & pluginLoopback,
                    const UKControllerPlugin::Controller::Login & login,
                    UKControllerPlugin::TimedEvent::DeferredEventHandler & deferredEvents,
                    bool automaticAssignmentAllowed
                );
                ~SquawkEventHandler();
                void FlightPlanEvent(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightPlan,
                    UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface & radarTarget
                ) override;
                void FlightPlanDisconnectEvent(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightPlan
                ) override;
                void ControllerFlightPlanDataEvent(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightPlan,
                    int dataType
                ) override;
                void SquawkReycleGeneral(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightPlan,
                    UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface & radarTarget
                ) const;
                void SquawkRecycleLocal(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightPlan,
                    UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface & radarTarget
                ) const;
                void TimedEventTrigger(void);

                // The callback function ID with euroscope for forcing squawk refresh (general squawk).
                const int squawkForceCallbackIdGeneral = 9000;

                // The callback function ID with euroscope for forcing squawk refresh (local squawk).
                const int squawkForceCallbackIdLocal = 9001;

                // Whether or not the automatic assignment of squawks is disabled (ie, not forced by user).
                const bool automaticAssignmentDisabled = true;

            private:

                // Generates squawks
                UKControllerPlugin::Squawk::SquawkGenerator & generator;

                // All the UK callsigns active on the network
                const UKControllerPlugin::Controller::ActiveCallsignCollection & activeCallsigns;

                // Stored flightplans
                const UKControllerPlugin::Flightplan::StoredFlightplanCollection & storedFlightplans;

                // Handles requests to the plugin core.
                UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & pluginLoopback;

                // If an event needs deferring, put it here.
                UKControllerPlugin::TimedEvent::DeferredEventHandler & deferredEvents;

                // Duration we have to be logged in to start doing automatic squawk assignments.
                const std::chrono::seconds minAutomaticAssignmentLoginTime;

                // Used for knowing if we're logged in and for how long.
                const UKControllerPlugin::Controller::Login & login;
        };
    }  // namespace Squawk
}  // namespace UKControllerPlugin
