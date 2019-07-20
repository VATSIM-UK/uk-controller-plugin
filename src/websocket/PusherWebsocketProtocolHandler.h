#pragma once
#include "timedevent/AbstractTimedEvent.h"
#include "websocket/WebsocketConnection.h"
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
                    UKControllerPlugin::TaskManager::TaskRunnerInterface & taskRunner
                );

            private:

                // The websocket itself
                UKControllerPlugin::Websocket::WebsocketConnectionInterface & websocket;

                // The web API
                const UKControllerPlugin::Api::ApiInterface & api;

                // Runs tasks off the main ES thread
                UKControllerPlugin::TaskManager::TaskRunnerInterface & taskRunner;

                // Inherited via AbstractTimedEvent
                virtual void TimedEventTrigger(void) override;
        };
    }  // namespace Websocket
}  // namespace UKControllerPlugin
