#pragma once
#include "push/PushEvent.h"
#include "push/PushEventSubscription.h"

namespace UKControllerPlugin::Push {

    /*
        An interface to be implemented by classes that
        want to process events pushed from the API.
    */
    class PushEventProcessorInterface
    {
        public:
        PushEventProcessorInterface() = default;
        virtual ~PushEventProcessorInterface() = default;
        PushEventProcessorInterface(const PushEventProcessorInterface&) = default;
        PushEventProcessorInterface(PushEventProcessorInterface&&) = default;
        auto operator=(const PushEventProcessorInterface&) -> PushEventProcessorInterface& = default;
        auto operator=(PushEventProcessorInterface&&) -> PushEventProcessorInterface& = default;

        /*
            Process a single push event message
        */
        virtual void ProcessPushEvent(const PushEvent& message) = 0;

        /*
            When the initial sync of plugin events is made, let processors know
            so that they can pre-load initial data, etc.
        */
        virtual void PluginEventsSynced(){};

        /*
            Return the channels that we want to subscribe to with this processor
        */
        [[nodiscard]] virtual auto GetPushEventSubscriptions() const -> std::set<PushEventSubscription> = 0;
    };
} // namespace UKControllerPlugin::Push
