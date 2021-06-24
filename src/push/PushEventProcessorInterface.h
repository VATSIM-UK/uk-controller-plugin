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
                    Process a message
                */
                virtual void ProcessWebsocketMessage(
                    const PushEvent& message
                ) = 0;

                /*
                    Return the channels that we want to subscribe to with this processor
                */
                virtual std::set<PushEventSubscription>
                    GetSubscriptions(void) const = 0;
        };
    } // namespace Push
}  // namespace UKControllerPlugin
