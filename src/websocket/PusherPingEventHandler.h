#pragma once
#include "websocket/WebsocketEventProcessorInterface.h"
#include "websocket/WebsocketConnectionInterface.h"

namespace UKControllerPlugin {
    namespace Websocket {

        class PusherPingEventHandler : public UKControllerPlugin::Websocket::WebsocketEventProcessorInterface
        {

            public:

                explicit PusherPingEventHandler(
                    UKControllerPlugin::Websocket::WebsocketConnectionInterface & websocket
                );

                // Inherited via WebsocketEventProcessorInterface
                void ProcessWebsocketMessage(const WebsocketMessage & message) override;
                std::set<WebsocketSubscription> GetSubscriptions(void) const override;

            private:

                // Allows us to send messages
                UKControllerPlugin::Websocket::WebsocketConnectionInterface & websocket;
        };

    }  // namespace Websocket
}  // namespace UKControllerPlugin
