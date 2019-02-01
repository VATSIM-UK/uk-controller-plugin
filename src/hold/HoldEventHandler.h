#pragma once
#include "timedevent/AbstractTimedEvent.h"
#include "flightplan/FlightPlanEventHandlerInterface.h"

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
    }  // namespace Euroscope
    namespace Hold {
        class HoldManager;
    }  // namespace Hold
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Hold {

        /*
            Handles events that relate to holding - namely the periodic
            update of holding data.
        */
        class HoldEventHandler : public UKControllerPlugin::Flightplan::FlightPlanEventHandlerInterface,
            public UKControllerPlugin::TimedEvent::AbstractTimedEvent
        {
            public:
                // Inherited via FlightPlanEventHandlerInterface
                HoldEventHandler(
                    UKControllerPlugin::Hold::HoldManager & holdManager,
                    UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin
                );
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

                // Inherited via AbstractTimedEvent
                void TimedEventTrigger(void) override;

            private:
                
                // Gives access to the plugin
                UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin;

                // Manages holds
                UKControllerPlugin::Hold::HoldManager & holdManager;
        };
    }  // namespace Hold
}  // namespace UKControllerPlugin
