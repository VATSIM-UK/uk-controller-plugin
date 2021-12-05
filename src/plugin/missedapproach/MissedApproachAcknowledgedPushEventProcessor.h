#pragma once
#include "push/PushEventProcessorInterface.h"

namespace UKControllerPlugin::Message {
    class UserMessager;
} // namespace UKControllerPlugin::Message

namespace UKControllerPlugin::MissedApproach {
    class MissedApproachCollection;

    class MissedApproachAcknowledgedPushEventProcessor : public Push::PushEventProcessorInterface
    {
        public:
        MissedApproachAcknowledgedPushEventProcessor(
            const MissedApproachCollection& missedApproaches, Message::UserMessager& messager);
        void ProcessPushEvent(const Push::PushEvent& message) override;
        [[nodiscard]] auto GetPushEventSubscriptions() const -> std::set<Push::PushEventSubscription> override;

        private:
        [[nodiscard]] static auto MessageValid(const nlohmann::json& message) -> bool;

        // All the approaches
        const MissedApproachCollection& missedApproaches;

        // Lets us send messages to user
        Message::UserMessager& messager;
    };
} // namespace UKControllerPlugin::MissedApproach
