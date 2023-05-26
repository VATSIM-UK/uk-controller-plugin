#pragma once
#include "radarscreen/ConfigurableDisplayInterface.h"
#include "windows/WinApiInterface.h"

namespace UKControllerPluginUtils::Api {
    class ApiSettingsProviderInterface;
} // namespace UKControllerPluginUtils::Api

namespace UKControllerPlugin {
    namespace Dialog {
        class DialogManager;
    } // namespace Dialog
    namespace Window {
        class WinApiInterface;
    } // namespace Window
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Api {

    /*
        A menu item in the plugin configuration menu
        that allows users to override their API configuration
        without having to perform manual steps.
    */
    class ApiConfigurationMenuItem : public UKControllerPlugin::RadarScreen::ConfigurableDisplayInterface
    {
        public:
        ApiConfigurationMenuItem(
            Dialog::DialogManager& dialogManager, Windows::WinApiInterface& windows, int menuCallbackId);

        // Inherited via ConfigurableDisplayInterface
        void Configure(int functionId, std::string subject, RECT area) override;
        [[nodiscard]] auto GetConfigurationMenuItem() const -> UKControllerPlugin::Plugin::PopupMenuItem override;

        private:
        // The item description
        const std::string itemDescription = "Replace Personal API Configuration";

        // Dialogs
        Dialog::DialogManager& dialogManager;

        // Windows API for the dialogs
        Windows::WinApiInterface& windows;

        // The id of the callback function for when the menu item is clicked
        const int menuCallbackId;
    };
} // namespace UKControllerPlugin::Api
