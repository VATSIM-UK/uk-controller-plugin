#pragma once
#include "push/PushEventProcessorInterface.h"

namespace UKControllerPlugin::Prenote {
    class PrenoteMessageCollection;

    class NewPrenotePushEventHandler : public UKControllerPlugin::Push::PushEventProcessorInterface
    {
        public:
        explicit NewPrenotePushEventHandler(std::shared_ptr<PrenoteMessageCollection> prenotes);
        void ProcessPushEvent(const Push::PushEvent& message) override;
        [[nodiscard]] auto GetPushEventSubscriptions() const
            -> std::set<UKControllerPlugin::Push::PushEventSubscription> override;

        private:
        [[nodiscard]] static auto MessageValid(const nlohmann::json& message) -> bool;

        // All the prenotes
        const std::shared_ptr<PrenoteMessageCollection> prenotes;
    };
} // namespace UKControllerPlugin::Prenote
