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
            WebsocketEventProcessorCollection & processors
        ) : websocket(websocket), processors(processors)
        {
        }

        /*
            Every time this event triggers, check for messages and hand
            them off to their processors.
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
                    this->processors.ProcessProtocolEvent(translatedMessage);
                } else {
                    this->processors.ProcessEvent(translatedMessage);
                }
            }
        }
    }  // namespace Websocket
}  // namespace UKControllerPlugin
