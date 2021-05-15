#pragma once
#include "radarscreen/ConfigurableDisplayInterface.h"
#include "command/CommandHandlerInterface.h"
#include "windows/WinApiInterface.h"

namespace UKControllerPlugin {
    namespace Plugin {

        /*
            Handler to appear in the bottom of the plugin menu. Triggers the
            users web browser to load the UKCP changelog.
        */
        class PluginChangelog : public RadarScreen::ConfigurableDisplayInterface,
                                public Command::CommandHandlerInterface
        {
            public:
                PluginChangelog(
                    Windows::WinApiInterface& winApi,
                    int menuCallbackId
                );

                // Inherited via ConfigurableDisplayInterface
                void Configure(int functionId, std::string subject, RECT screenObjectArea) override;
                PopupMenuItem GetConfigurationMenuItem(void) const override;

                // Inherited via CommandHandlerInterface
                bool ProcessCommand(std::string command) override;

                // The command to accept
                const std::string command = ".ukcp changelog";

                // The description of the menu item
                const std::string menuItemDescription = "UKCP Changelog (Opens In Web Browser)";

            private:

                void ShowChangelog() const;

                // The ID of the callback
                const int menuCallbackId;

                // To windows so we can do the message
                Windows::WinApiInterface& winApi;
        };
    }  // namespace Plugin
}  // namespace UKControllerPlugin
