#pragma once
#include "radarscreen/ConfigurableDisplayInterface.h"

namespace UKControllerPlugin::RadarScreen {
    class MenuToggleableDisplayInterface;

    class ToggleDisplayFromMenu : public ConfigurableDisplayInterface
    {
        public:
        ToggleDisplayFromMenu(std::shared_ptr<MenuToggleableDisplayInterface> display, int callbackId);
        void Configure(int functionId, std::string subject, RECT screenObjectArea) override;
        auto GetConfigurationMenuItem() const -> UKControllerPlugin::Plugin::PopupMenuItem override;

        private:
        // The display that we can toggle
        const std::shared_ptr<MenuToggleableDisplayInterface> display;

        // The callback id
        int callbackId;
    };
} // namespace UKControllerPlugin::RadarScreen
