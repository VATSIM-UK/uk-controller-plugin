#pragma once
#include "integration/IntegrationActionProcessor.h"

namespace UKControllerPlugin::Euroscope {
    class EuroscopePluginLoopbackInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::MissedApproach {
    class TriggerMissedApproach;

    /**
     * Handles messages from integrations to trigger missed approaches
     */
    class TriggerMissedApproachMessageHandler : public Integration::IntegrationActionProcessor
    {
        public:
        TriggerMissedApproachMessageHandler(
            const TriggerMissedApproach& trigger, const Euroscope::EuroscopePluginLoopbackInterface& plugin);
        [[nodiscard]] auto ActionsToProcess() const -> std::vector<Integration::MessageType> override;
        void ProcessAction(
            std::shared_ptr<Integration::MessageInterface> message,
            std::function<void(void)> success,
            std::function<void(std::vector<std::string>)> fail) override;

        private:
        // Triggers missed approaches
        const TriggerMissedApproach& trigger;

        // Plugin for getting flightplans and radar targets
        const Euroscope::EuroscopePluginLoopbackInterface& plugin;
    };
} // namespace UKControllerPlugin::MissedApproach
