#pragma once
#include "websocket/WebsocketEventProcessorInterface.h"
#include "websocket/WebsocketConnectionInterface.h"
#include "windows/WinApiInterface.h"

namespace UKControllerPlugin {
    namespace Websocket {


        /*
            A class that logs any errors that come from the websocket.
        */
        class PusherErrorEventHandler : public UKControllerPlugin::Websocket::WebsocketEventProcessorInterface
        {
            public:

                // Inherited via WebsocketEventProcessorInterface
                void ProcessWebsocketMessage(const UKControllerPlugin::Websocket::WebsocketMessage & message) override;
                std::set<WebsocketSubscription> GetSubscriptions(void) const override;
        };
    }  // namespace Websocket
}  // namespace UKControllerPlugin
