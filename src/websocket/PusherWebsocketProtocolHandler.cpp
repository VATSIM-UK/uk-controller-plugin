    #include "pch/stdafx.h"
#include "websocket/PusherWebsocketProtocolHandler.h"
#include "websocket/InterpretPusherWebsocketMessage.h"

using UKControllerPlugin::Websocket::WebsocketConnectionInterface;
using UKControllerPlugin::Websocket::WebsocketEventProcessorCollection;
using UKControllerPlugin::Api::ApiInterface;
using UKControllerPlugin::TaskManager::TaskRunnerInterface;

namespace UKControllerPlugin {
    namespace Websocket {

        PusherWebsocketProtocolHandler::PusherWebsocketProtocolHandler(
            WebsocketConnectionInterface & websocket,
            const ApiInterface & api, 
            TaskRunnerInterface & taskRunner,
            WebsocketEventProcessorCollection & processors
        ) : websocket(websocket), api(api), taskRunner(taskRunner), processors(processors)
        {
        }

        /*
            Every time this event triggers, check for messages, process them
            and handle the rest of the protocol.
        */
        void PusherWebsocketProtocolHandler::TimedEventTrigger(void)
        {
            std::string incomingMessage;

            while ((incomingMessage = this->websocket.GetNextMessage()) != this->websocket.noMessage) {
                WebsocketMessage translatedMessage = InterpretPusherMessage(incomingMessage);

                if (translatedMessage == invalidMessage) {
                    continue;
                }

                if (translatedMessage.protocolMessage) {
                    this->HandleProtocolMessage(translatedMessage);
                    continue;
                }

                this->processors.ProcessEvent(translatedMessage);
            }
        }

        void PusherWebsocketProtocolHandler::HandleProtocolMessage(WebsocketMessage & message)
        {
            if (message.event == "pusher:ping") {
                nlohmann::json responseJson;
                responseJson["event"] = "pusher:pong";
                this->websocket.WriteMessage(responseJson.dump());
            }
        }
    }  // namespace Websocket
}  // namespace UKControllerPlugin
