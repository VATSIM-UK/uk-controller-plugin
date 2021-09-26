#pragma once
#include "push/PushEventProcessorInterface.h"

namespace UKControllerPlugin::MissedApproach {
    class MissedApproachCollection;

    /**
     * Handles the new missed approach event to add a missed
     * approach to the collection-
     */
    class NewMissedApproachPushEventHandler : public Push::PushEventProcessorInterface
    {
        public:
        NewMissedApproachPushEventHandler(std::shared_ptr<MissedApproachCollection> missedApproaches);
        void ProcessPushEvent(const Push::PushEvent& message) override;
        [[nodiscard]] auto GetPushEventSubscriptions() const -> std::set<Push::PushEventSubscription> override;

        private:
        [[nodiscard]] static auto MessageValid(const nlohmann::json& data) -> bool;
        [[nodiscard]] auto CheckForExisting(int id) const -> bool;

        const std::shared_ptr<MissedApproachCollection> missedApproaches;
    };
} // namespace UKControllerPlugin::MissedApproach
