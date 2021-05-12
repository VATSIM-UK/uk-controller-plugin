#pragma once
#include "websocket/WebsocketMessage.h"
#include "websocket/WebsocketSubscription.h"

namespace UKControllerPlugin {
    namespace Websocket {

        /*
            An interface to be implemented by classes that
            want to process websocket messages.
        */
        class WebsocketEventProcessorInterface
        {
            public:
                virtual ~WebsocketEventProcessorInterface() = default;

                /*
                    Process a message
                */
                virtual void ProcessWebsocketMessage(
                    const UKControllerPlugin::Websocket::WebsocketMessage & message
                ) = 0;

                /*
                    Return the channels that we want to subscribe to with this processor
                */
                virtual std::set<UKControllerPlugin::Websocket::WebsocketSubscription>
                    GetSubscriptions(void) const = 0;
        };
    }  // namespace Websocket
}  // namespace UKControllerPlugin
