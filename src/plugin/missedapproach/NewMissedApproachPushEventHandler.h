#pragma once
#include "push/PushEventProcessorInterface.h"

namespace UKControllerPlugin::MissedApproach {
    class MissedApproachCollection;
    class MissedApproachAudioAlert;

    /**
     * Handles the new missed approach event to add a missed
     * approach to the collection-
     */
    class NewMissedApproachPushEventHandler : public Push::PushEventProcessorInterface
    {
        public:
        NewMissedApproachPushEventHandler(
            std::shared_ptr<MissedApproachCollection> missedApproaches,
            std::shared_ptr<const MissedApproachAudioAlert> audioAlert);
        void ProcessPushEvent(const Push::PushEvent& message) override;
        [[nodiscard]] auto GetPushEventSubscriptions() const -> std::set<Push::PushEventSubscription> override;

        private:
        [[nodiscard]] static auto MessageValid(const nlohmann::json& data) -> bool;
        [[nodiscard]] auto CheckForExisting(int id) const -> bool;

        // All the approaches
        const std::shared_ptr<MissedApproachCollection> missedApproaches;

        // For alerting the user
        const std::shared_ptr<const MissedApproachAudioAlert> audioAlert;
    };
} // namespace UKControllerPlugin::MissedApproach
