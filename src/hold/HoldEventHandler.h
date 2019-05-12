#pragma once
#include "timedevent/AbstractTimedEvent.h"
#include "flightplan/FlightPlanEventHandlerInterface.h"
#include "radarscreen/ConfigurableDisplayInterface.h"
#include "command/CommandHandlerInterface.h"
#include "tag/TagItemInterface.h"

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
            public UKControllerPlugin::TimedEvent::AbstractTimedEvent,
            public UKControllerPlugin::Tag::TagItemInterface
        {
            public:
                // Inherited via FlightPlanEventHandlerInterface
                HoldEventHandler(
                    UKControllerPlugin::Hold::HoldManager & holdManager,
                    UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin,
                    const int popupMenuItemId
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

                // Inherited via TagItemInterface
                std::string GetTagItemDescription(void) const override;
                std::string GetTagItemData(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightPlan,
                    UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface & radarTarget
                ) override;


                // The string to display when an aircraft is not holding
                const std::string noHold = "NOHOLD";

                // The id of this handlers popup menu item
                const int popupMenuItemId;

            private:

                // Gives access to the plugin
                UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin;

                // Manages holds
                UKControllerPlugin::Hold::HoldManager & holdManager;
        };
    }  // namespace Hold
}  // namespace UKControllerPlugin
