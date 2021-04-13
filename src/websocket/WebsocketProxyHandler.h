#pragma once
#include "websocket/WebsocketEventProcessorInterface.h"
#include "websocket/WebsocketConnectionInterface.h"

namespace UKControllerPlugin {
    namespace Websocket {

        class WebsocketProxyHandler : public WebsocketEventProcessorInterface
        {

            public:

                // Inherited via WebsocketEventProcessorInterface
                void ProcessWebsocketMessage(const WebsocketMessage & message) override;
                std::set<WebsocketSubscription> GetSubscriptions(void) const override;
        };

    }  // namespace Websocket
}  // namespace UKControllerPlugin
