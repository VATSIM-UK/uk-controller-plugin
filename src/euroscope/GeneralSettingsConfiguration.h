#pragma once
#include "radarscreen/ConfigurableDisplayInterface.h"
#include "command/CommandHandlerInterface.h"

namespace UKControllerPlugin {
    namespace Windows {
        class WinApiInterface;
    }  // namespace Windows
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
                    UKControllerPlugin::Windows::WinApiInterface & winApi,
                    const int menuSelectedCallbackId
                );

                // Inherited via CommandHandlerInterface
                bool ProcessCommand(std::string command) override;

                // Inherited via ConfigurableDisplayInterface
                void Configure(std::string subject) override;
                UKControllerPlugin::Plugin::PopupMenuItem GetConfigurationMenuItem(void) const override;

                // The description to display in the configuration popup menu.
                const std::string configMenuDescription = "General Settings";

                // The dialogId to load
                const int dialogId = IDD_GENERAL_SETTINGS;

                // The euroscope function id this uses for when the menu item is clicked
                const int menuSelectedCallbackId;

            private:

                // A link to the Windows API
                UKControllerPlugin::Windows::WinApiInterface & winApi;
        };
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
