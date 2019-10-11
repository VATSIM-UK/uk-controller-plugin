#pragma once
#include "websocket/WebsocketEventProcessorInterface.h"
#include "websocket/WebsocketConnectionInterface.h"

namespace UKControllerPlugin {
    namespace Websocket {


        /*
            A class that handles activity timeouts from the websocket and
            makes sure the connection is well maintained.
        */
        class PusherActivityTimeoutEventHandler :
            public UKControllerPlugin::Websocket::WebsocketEventProcessorInterface
        {
            public:

                PusherActivityTimeoutEventHandler(
                    UKControllerPlugin::Websocket::WebsocketConnectionInterface & websocket
                );

                // Inherited via WebsocketEventProcessorInterface
                void ProcessWebsocketMessage(const UKControllerPlugin::Websocket::WebsocketMessage & message) override;
                std::set<WebsocketSubscription> GetSubscriptions(void) const override;

                // How often should we ping by default
                const std::chrono::seconds defaultTimeoutInterval;

            private:

                // Talking to the websocket
                UKControllerPlugin::Websocket::WebsocketConnectionInterface & websocket;
        };
    }  // namespace Websocket
}  // namespace UKControllerPlugin
