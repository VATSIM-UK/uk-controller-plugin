#pragma once
#include "websocket/WebsocketMessage.h"

namespace UKControllerPlugin {
    namespace Websocket {

        /*
            An interface to be implemented by classes that
            want to process websocket message.
        */
        class WebsocketEventProcessorInterface
        {
            public:
                
                /*
                    Process a message
                */
                virtual void ProcessWebsocketMessage(
                    const UKControllerPlugin::Websocket::WebsocketMessage & message
                ) = 0;

                /*
                    Return the channels that we want to subscribe to with this processor
                */
                virtual std::set<std::string> GetSubscriptions(void) const = 0;
        };
    }  // namespace Websocket
}  // namespace UKControllerPlugin
