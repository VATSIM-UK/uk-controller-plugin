#pragma once
#include "websocket/WebsocketMessage.h"

namespace UKControllerPlugin {
    namespace Websocket {

        /*
            An interface to be implemented by classes that handle
            the websocket protocol.
        */
        class WebsocketProtocolProcessorInterface
        {
            public:
                
                /*
                    Process a message
                */
                virtual void ProcessProtocolMessage(
                    const UKControllerPlugin::Websocket::WebsocketMessage & message
                ) = 0;

                /*
                    Return the events we want the processor to process.
                */
                virtual std::set<std::string> GetEventSubscriptions(void) const = 0;
        };
    }  // namespace Websocket
}  // namespace UKControllerPlugin
