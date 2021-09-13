#pragma once
#include "radarscreen/ConfigurableDisplayInterface.h"
#include "windows/WinApiInterface.h"

namespace UKControllerPlugin::Api {

    /*
        A menu item in the plugin configuration menu
        that allows users to override their API configuration
        without having to perform manual steps.
    */
    class ApiConfigurationMenuItem : public UKControllerPlugin::RadarScreen::ConfigurableDisplayInterface
    {
        public:
        ApiConfigurationMenuItem(UKControllerPlugin::Windows::WinApiInterface& winApi, int menuCallbackId);

        // Inherited via ConfigurableDisplayInterface
        void Configure(int functionId, std::string subject, RECT area) override;
        [[nodiscard]] auto GetConfigurationMenuItem() const -> UKControllerPlugin::Plugin::PopupMenuItem override;

        private:
        // The item description
        const std::string itemDescription = "Replace Personal API Configuration";

        // The id of the callback function for when the menu item is clicked
        const int menuCallbackId;

        // The windows API
        UKControllerPlugin::Windows::WinApiInterface& winApi;
    };
} // namespace UKControllerPlugin::Api
