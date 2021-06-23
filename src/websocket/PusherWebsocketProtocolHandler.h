#pragma once
#include "timedevent/AbstractTimedEvent.h"
#include "websocket/WebsocketConnectionInterface.h"
#include "websocket/WebsocketEventProcessorCollection.h"
#include "api/ApiInterface.h"
#include "task/TaskRunnerInterface.h"

namespace UKControllerPlugin {
    namespace Websocket {

        /*
            Handles the pusher websocket protocol
            and processes incoming messages.
        */
        class PusherWebsocketProtocolHandler : public TimedEvent::AbstractTimedEvent
        {
            public:
                PusherWebsocketProtocolHandler(
                    WebsocketConnectionInterface& websocket,
                    WebsocketEventProcessorCollection& processors
                );

                // Inherited via AbstractTimedEvent
                void TimedEventTrigger(void) override;

            private:

                // The websocket itself
                WebsocketConnectionInterface& websocket;

                // Process websocket events
                WebsocketEventProcessorCollection& processors;
        };
    }  // namespace Websocket
}  // namespace UKControllerPlugin
