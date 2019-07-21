#pragma once
#include "timedevent/AbstractTimedEvent.h"
#include "websocket/WebsocketConnection.h"
#include "websocket/WebsocketEventProcessorCollection.h"
#include "api/ApiInterface.h"
#include "task/TaskRunnerInterface.h"

namespace UKControllerPlugin {
    namespace Websocket {

        /*
            Handles the pusher websocket protocol
            and processes incoming messages.
        */
        class PusherWebsocketProtocolHandler : public UKControllerPlugin::TimedEvent::AbstractTimedEvent
        {
            public:
                PusherWebsocketProtocolHandler(
                    UKControllerPlugin::Websocket::WebsocketConnectionInterface & websocket,
                    UKControllerPlugin::Websocket::WebsocketEventProcessorCollection & processors
                );

                // Inherited via AbstractTimedEvent
                virtual void TimedEventTrigger(void) override;

            private:

                // The websocket itself
                UKControllerPlugin::Websocket::WebsocketConnectionInterface & websocket;

                // Process websocket events
                UKControllerPlugin::Websocket::WebsocketEventProcessorCollection & processors;
        };
    }  // namespace Websocket
}  // namespace UKControllerPlugin
