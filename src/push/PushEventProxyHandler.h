#pragma once
#include "push/PushEventProcessorInterface.h"
#include "push/PushEventConnectionInterface.h"

namespace UKControllerPlugin {
    namespace Push {

        class PushEventProxyHandler : public PushEventProcessorInterface
        {
            public:
                void ProcessPushEvent(const PushEvent& message) override;
                std::set<PushEventSubscription> GetPushEventSubscriptions(void) const override;
        };

    } // namespace Push
} // namespace UKControllerPlugin
