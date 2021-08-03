#pragma once
#include "timedevent/AbstractTimedEvent.h"
#include "flightplan/FlightPlanEventHandlerInterface.h"
#include "euroscope/UserSettingAwareInterface.h"
#include "controller/ActiveCallsignEventHandlerInterface.h"

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
} // namespace UKControllerPlugin


namespace UKControllerPlugin {
    namespace Squawk {

        class SquawkEventHandler : public UKControllerPlugin::Flightplan::FlightPlanEventHandlerInterface,
            public UKControllerPlugin::TimedEvent::AbstractTimedEvent,
            public UKControllerPlugin::Euroscope::UserSettingAwareInterface,
            public UKControllerPlugin::Controller::ActiveCallsignEventHandlerInterface
        {
            public:
                SquawkEventHandler(
                    UKControllerPlugin::Squawk::SquawkGenerator & generator,
                    const UKControllerPlugin::Controller::ActiveCallsignCollection & activeCallsigns,
                    const UKControllerPlugin::Flightplan::StoredFlightplanCollection & storedFlightplans,
                    UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & pluginLoopback,
                    const UKControllerPlugin::Controller::Login & login,
                    bool automaticAssignmentAllowed
                );
                ~SquawkEventHandler() override = default;
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
                    UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface & radarTarget,
                    std::string context,
                    const POINT & mousePos
                ) const;
                void SquawkRecycleLocal(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightPlan,
                    UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface & radarTarget,
                    std::string context,
                    const POINT & mousePos
                ) const;
                void TimedEventTrigger(void);
                void UserSettingsUpdated(UKControllerPlugin::Euroscope::UserSetting & userSettings) override;
                bool UserAllowedSquawkAssignment(void) const;

                // Inherited via ActiveCallsignEventHandlerInterface
                void ActiveCallsignAdded(
                    const UKControllerPlugin::Controller::ActiveCallsign& callsign,
                    bool userCallsign
                ) override;
                void ActiveCallsignRemoved(
                    const UKControllerPlugin::Controller::ActiveCallsign& callsign,
                    bool userCallsign
                ) override;
                void CallsignsFlushed(void) override;

                // The callback function ID with euroscope for forcing squawk refresh (general squawk).
                const int squawkForceCallbackIdGeneral = 9000;

                // The callback function ID with euroscope for forcing squawk refresh (local squawk).
                const int squawkForceCallbackIdLocal = 9001;

                // Whether or not the automatic assignment of squawks is disabled (ie, not forced by user).
                const bool automaticAssignmentDisabled = true;

            private:
                void AttemptAssignment(
                    Euroscope::EuroScopeCFlightPlanInterface& flightplan,
                    Euroscope::EuroScopeCRadarTargetInterface& radarTarget
                ) const;

                void AttemptAssignSquawksToAllAircraft() const;

                // Generates squawks
                UKControllerPlugin::Squawk::SquawkGenerator & generator;

                // All the UK callsigns active on the network
                const UKControllerPlugin::Controller::ActiveCallsignCollection & activeCallsigns;

                // Stored flightplans
                const UKControllerPlugin::Flightplan::StoredFlightplanCollection & storedFlightplans;

                // Handles requests to the plugin core.
                UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & pluginLoopback;

                // Duration we have to be logged in to start doing automatic squawk assignments.
                const std::chrono::seconds minAutomaticAssignmentLoginTime;

                // Used for knowing if we're logged in and for how long.
                const UKControllerPlugin::Controller::Login & login;

                // Whether or not the user has enabled automatic squawk assignment
                bool userAutomaticAssignmentEnabled = true;
        };
    }  // namespace Squawk
}  // namespace UKControllerPlugin
