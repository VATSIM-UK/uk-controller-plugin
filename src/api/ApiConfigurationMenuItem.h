#pragma once
#include "radarscreen/ConfigurableDisplayInterface.h"
#include "windows/WinApiInterface.h"

namespace UKControllerPlugin {
    namespace Api {

        /*
            A menu item in the plugin configuration menu
            that allows users to override their API configuration
            without having to perform manual steps.
        */
        class ApiConfigurationMenuItem : public UKControllerPlugin::RadarScreen::ConfigurableDisplayInterface
        {
            public:
                
                ApiConfigurationMenuItem(
                    UKControllerPlugin::Windows::WinApiInterface & winApi,
                    unsigned int menuCallbackId
                );

                // Inherited via ConfigurableDisplayInterface
                void Configure(std::string subject) override;
                UKControllerPlugin::Plugin::PopupMenuItem GetConfigurationMenuItem(void) const override;

                // The item description
                const std::string itemDescription = "Replace Personal API Configuration";

            private:

                // The id of the callback function for when the menu item is clicked
                const unsigned int menuCallbackId;
                
                // The windows API
                UKControllerPlugin::Windows::WinApiInterface & winApi;
        };
    }  // namespace Api
}  // namespace UKControllerPlugin
