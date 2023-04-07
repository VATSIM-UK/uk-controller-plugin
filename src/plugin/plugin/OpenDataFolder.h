#pragma once
#include "radarscreen/ConfigurableDisplayInterface.h"

namespace UKControllerPlugin::Windows {
    class WinApiInterface;
} // namespace UKControllerPlugin::Windows

namespace UKControllerPlugin::Plugin {
    class OpenDataFolder : public RadarScreen::ConfigurableDisplayInterface
    {
        public:
        OpenDataFolder(const Windows::WinApiInterface& windows, int menuCallbackId);
        void Configure(int functionId, std::string subject, RECT screenObjectArea) override;
        auto GetConfigurationMenuItem() const -> UKControllerPlugin::Plugin::PopupMenuItem override;

        private:
        // For opening the explorer window
        const Windows::WinApiInterface& windows;

        // Callback id
        const int menuCallbackId;
    };
} // namespace UKControllerPlugin::Plugin
