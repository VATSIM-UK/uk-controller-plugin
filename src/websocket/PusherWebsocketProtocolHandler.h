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
                    const UKControllerPlugin::Api::ApiInterface & api,
                    UKControllerPlugin::TaskManager::TaskRunnerInterface & taskRunner,
                    UKControllerPlugin::Websocket::WebsocketEventProcessorCollection & processors
                );

                // Inherited via AbstractTimedEvent
                virtual void TimedEventTrigger(void) override;

            private:

                void HandleProtocolMessage(UKControllerPlugin::Websocket::WebsocketMessage & message);

                // The websocket itself
                UKControllerPlugin::Websocket::WebsocketConnectionInterface & websocket;

                // The web API
                const UKControllerPlugin::Api::ApiInterface & api;

                // Runs tasks off the main ES thread
                UKControllerPlugin::TaskManager::TaskRunnerInterface & taskRunner;

                // Process websocket events
                UKControllerPlugin::Websocket::WebsocketEventProcessorCollection & processors;
        };
    }  // namespace Websocket
}  // namespace UKControllerPlugin
