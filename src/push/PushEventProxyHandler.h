#pragma once
#include "push/PushEventProcessorInterface.h"
#include "push/PushEventConnectionInterface.h"

namespace UKControllerPlugin {
    namespace Push {

        class PushEventProxyHandler : public PushEventProcessorInterface
        {
            public:
                void ProcessWebsocketMessage(const PushEvent& message) override;
                std::set<PushEventSubscription> GetSubscriptions(void) const override;
        };

    } // namespace Push
} // namespace UKControllerPlugin
