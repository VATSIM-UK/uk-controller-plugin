#pragma once
#include "timedevent/AbstractTimedEvent.h"
#include "push/PushEventConnectionInterface.h"
#include "push/PushEventProcessorCollection.h"

namespace UKControllerPlugin {
    namespace Push {

        /*
            Handles the push event protocol and deals with
            incoming messages
        */
        class PushEventProtocolHandler : public TimedEvent::AbstractTimedEvent
        {
            public:
                PushEventProtocolHandler(
                    PushEventConnectionInterface& pushEvents,
                    PushEventProcessorCollection& processors
                );

                // Inherited via AbstractTimedEvent
                void TimedEventTrigger(void) override;

            private:
                // The connection for push events
                PushEventConnectionInterface& pushEvents;

                // Processes push events
                PushEventProcessorCollection& processors;
        };
    } // namespace Push
}  // namespace UKControllerPlugin
