#pragma once
#include "push/PushEventProcessorInterface.h"
#include "push/PushEventConnectionInterface.h"

namespace UKControllerPlugin {
    namespace Push {

        class PushEventProxyHandler : public PushEventProcessorInterface
        {
            public:
                ~PushEventProxyHandler() override = default;
                void ProcessPushEvent(const PushEvent& message) override;
                std::set<PushEventSubscription> GetPushEventSubscriptions(void) const override;
                void PluginEventsSynced() override;
        };

    } // namespace Push
} // namespace UKControllerPlugin
