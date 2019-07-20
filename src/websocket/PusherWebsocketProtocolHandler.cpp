#include "pch/stdafx.h"
#include "websocket/PusherWebsocketProtocolHandler.h"

using UKControllerPlugin::Websocket::WebsocketConnectionInterface;
using UKControllerPlugin::Api::ApiInterface;
using UKControllerPlugin::TaskManager::TaskRunnerInterface;

namespace UKControllerPlugin {
    namespace Websocket {

        PusherWebsocketProtocolHandler::PusherWebsocketProtocolHandler(
            WebsocketConnectionInterface & websocket,
            const ApiInterface & api, 
            TaskRunnerInterface & taskRunner
        ) : websocket(websocket), api(api), taskRunner(taskRunner)
        {
        }

        void PusherWebsocketProtocolHandler::TimedEventTrigger(void)
        {

        }
    }  // namespace Websocket
}  // namespace UKControllerPlugin
