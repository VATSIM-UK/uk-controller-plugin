#pragma once
#include "radarscreen/ConfigurableDisplayInterface.h"
#include "command/CommandHandlerInterface.h"

namespace UKControllerPlugin {
    namespace Dialog {
        class DialogManager;
    }  // namespace Dialog
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Euroscope {

        /*
            A euroscope menu item which when clicked opens the general settings modal.
        */
        class GeneralSettingsConfiguration : public UKControllerPlugin::RadarScreen::ConfigurableDisplayInterface,
            public UKControllerPlugin::Command::CommandHandlerInterface
        {
            public:
                GeneralSettingsConfiguration(
                    const UKControllerPlugin::Dialog::DialogManager & dialogManager,
                    const int menuSelectedCallbackId
                );

                // Inherited via CommandHandlerInterface
                bool ProcessCommand(std::string command) override;

                // Inherited via ConfigurableDisplayInterface
                void Configure(int functionId, std::string subject, RECT screenObjectArea) override;
                UKControllerPlugin::Plugin::PopupMenuItem GetConfigurationMenuItem(void) const override;

                // The description to display in the configuration popup menu.
                const std::string configMenuDescription = "General Settings";

                // The dialogId to load
                const int dialogId = IDD_GENERAL_SETTINGS;

                // The euroscope function id this uses for when the menu item is clicked
                const int menuSelectedCallbackId;

            private:

                // Manages dialogs
                const UKControllerPlugin::Dialog::DialogManager & dialogManager;
        };
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
