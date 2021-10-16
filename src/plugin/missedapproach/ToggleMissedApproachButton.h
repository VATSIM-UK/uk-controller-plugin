#pragma once
#include "radarscreen/ConfigurableDisplayInterface.h"

namespace UKControllerPlugin::MissedApproach {
    class MissedApproachButton;

    /**
     * Toggles the missed approach button for a given ASR through
     * an option in the OP menu.
     */
    class ToggleMissedApproachButton : public RadarScreen::ConfigurableDisplayInterface
    {
        public:
        ToggleMissedApproachButton(std::shared_ptr<MissedApproachButton> button, int configureCallbackId);
        void Configure(int functionId, std::string subject, RECT screenObjectArea) override;
        [[nodiscard]] auto GetConfigurationMenuItem() const -> UKControllerPlugin::Plugin::PopupMenuItem override;

        private:
        // The button we want to toggle
        const std::shared_ptr<MissedApproachButton> button;

        // Callback id for a click
        const int configureCallbackId;
    };
} // namespace UKControllerPlugin::MissedApproach
