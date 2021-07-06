#pragma once
#include "push/PushEvent.h"
#include "push/PushEventSubscription.h"

namespace UKControllerPlugin {
    namespace Push {

        /*
            An interface to be implemented by classes that
            want to process events pushed from the API.
        */
        class PushEventProcessorInterface
        {
            public:
                virtual ~PushEventProcessorInterface() = default;

                /*
                    Process a single push event message
                */
                virtual void ProcessPushEvent(
                    const PushEvent& message
                ) = 0;

                /*
                    When the initial sync of plugin events is made, let processors know
                    so that they can pre-load initial data, etc.
                */
                virtual void PluginEventsSynced() = 0;

                /*
                    Return the channels that we want to subscribe to with this processor
                */
                virtual std::set<PushEventSubscription>
                GetPushEventSubscriptions(void) const = 0;
        };
    } // namespace Push
}  // namespace UKControllerPlugin
