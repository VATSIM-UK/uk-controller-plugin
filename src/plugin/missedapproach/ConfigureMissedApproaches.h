#pragma once
#include "radarscreen/ConfigurableDisplayInterface.h"

namespace UKControllerPlugin::Dialog {
    class DialogManager;
} // namespace UKControllerPlugin::Dialog

namespace UKControllerPlugin::MissedApproach {
    class MissedApproachRenderOptions;

    /**
     * Toggles the missed approach button for a given ASR through
     * an option in the OP menu.
     */
    class ConfigureMissedApproaches : public RadarScreen::ConfigurableDisplayInterface
    {
        public:
        ConfigureMissedApproaches(
            std::shared_ptr<MissedApproachRenderOptions> renderOptions,
            Dialog::DialogManager& dialogManager,
            int configureCallbackId);
        void Configure(int functionId, std::string subject, RECT screenObjectArea) override;
        [[nodiscard]] auto GetConfigurationMenuItem() const -> UKControllerPlugin::Plugin::PopupMenuItem override;

        private:
        // The button we want to toggle
        std::shared_ptr<MissedApproachRenderOptions> renderOptions;

        // Opens dialog
        Dialog::DialogManager& dialogManager;

        // Callback id for a click
        const int configureCallbackId;
    };
} // namespace UKControllerPlugin::MissedApproach
