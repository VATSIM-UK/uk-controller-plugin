#pragma once
#include "command/CommandHandlerInterface.h"
#include "radarscreen/ConfigurableDisplayInterface.h"

namespace UKControllerPlugin::Windows {
    class WinApiInterface;
} // namespace UKControllerPlugin::Windows

namespace UKControllerPlugin::Plugin {

    /*
        Handler to appear in the bottom of the plugin menu
        to display an "About" message for UKCP.
    */
    class PluginInformationMessage : public UKControllerPlugin::RadarScreen::ConfigurableDisplayInterface,
                                     public UKControllerPlugin::Command::CommandHandlerInterface
    {
        public:
        PluginInformationMessage(UKControllerPlugin::Windows::WinApiInterface& winApi, int menuCallbackId);

        // Inherited via ConfigurableDisplayInterface
        void Configure(int functionId, std::string subject, RECT screenObjectArea) override;
        [[nodiscard]] auto GetConfigurationMenuItem() const -> UKControllerPlugin::Plugin::PopupMenuItem override;

        // Inherited via CommandHandlerInterface
        [[nodiscard]] auto ProcessCommand(std::string command) -> bool override;

        private:
        void ShowInformationMessage();

        // The ID of the callback
        const int menuCallbackId;

        // To windows so we can do the message
        UKControllerPlugin::Windows::WinApiInterface& winApi;

        const int DATE_BASE_YEAR = 1900;

        // The command to accept
        const std::string command = ".ukcp about";

        // The description of the menu item
        const std::string menuItemDescription = "About UKCP";
    };
} // namespace UKControllerPlugin::Plugin
