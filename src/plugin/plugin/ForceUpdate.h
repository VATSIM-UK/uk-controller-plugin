#pragma once
#include "radarscreen/ConfigurableDisplayInterface.h"
#include "command/CommandHandlerInterface.h"
#include "windows/WinApiInterface.h"

namespace UKControllerPlugin {
    namespace Plugin {

        /*
            Handler to appear in the bottom of the plugin menu. Triggers
            the plugin to perform a forced update on next load.
        */
        class ForceUpdate : public RadarScreen::ConfigurableDisplayInterface,
                            public Command::CommandHandlerInterface
        {
            public:
                ForceUpdate(
                    Windows::WinApiInterface& winApi,
                    int menuCallbackId
                );

                // Inherited via ConfigurableDisplayInterface
                void Configure(int functionId, std::string subject, RECT screenObjectArea) override;
                PopupMenuItem GetConfigurationMenuItem(void) const override;

                // Inherited via CommandHandlerInterface
                bool ProcessCommand(std::string command) override;

                // The command to accept
                const std::string command = ".ukcp forceupdate";

                // The description of the menu item
                const std::string menuItemDescription = "Force Plugin To Update";

            private:

                void ForcePluginUpdate() const;

                // The ID of the callback
                const int menuCallbackId;

                // To windows so we can do the message
                Windows::WinApiInterface& winApi;
        };
    }  // namespace Plugin
}  // namespace UKControllerPlugin
