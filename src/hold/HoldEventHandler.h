#pragma once
#include "timedevent/AbstractTimedEvent.h"
#include "flightplan/FlightPlanEventHandlerInterface.h"
#include "radarscreen/ConfigurableDisplayInterface.h"
#include "command/CommandHandlerInterface.h"
#include "hold/HoldWindowManager.h"

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
            public UKControllerPlugin::RadarScreen::ConfigurableDisplayInterface,
            public UKControllerPlugin::Command::CommandHandlerInterface
        {
            public:
                // Inherited via FlightPlanEventHandlerInterface
                HoldEventHandler(
                    UKControllerPlugin::Hold::HoldManager & holdManager,
                    UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin,
                    UKControllerPlugin::Hold::HoldWindowManager holdManagerWindow,
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

                // Inherited via ConfigurableDisplayInterface
                void Configure(std::string subject) override;
                UKControllerPlugin::Plugin::PopupMenuItem GetConfigurationMenuItem(void) const override;

                // Inherited via CommandHandlerInterface
                bool ProcessCommand(std::string command) override;

                // The id of this handlers popup menu item
                const int popupMenuItemId;

                // The command that toggles opening the hold manager
                const std::string toggleCommand = ".ukcp hold";

                // The description of the window item
                const std::string menuItemDescription = "Hold Manager Window";

            private:
                
                // Gives access to the plugin
                UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin;

                // Manages holds
                UKControllerPlugin::Hold::HoldManager & holdManager;

                // Displays the hold manager view
                UKControllerPlugin::Hold::HoldWindowManager holdManagerWindow;
        };
    }  // namespace Hold
}  // namespace UKControllerPlugin
