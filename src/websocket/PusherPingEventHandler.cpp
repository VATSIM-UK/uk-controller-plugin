#include "pch/stdafx.h"
#include "websocket/PusherPingEventHandler.h"

namespace UKControllerPlugin {
    namespace Websocket {

        PusherPingEventHandler::PusherPingEventHandler(WebsocketConnectionInterface & websocket)
            : websocket(websocket)
        {

        }

        /*
            Handle the ping messages by responding with pong.
        */
        void PusherPingEventHandler::ProcessWebsocketMessage(const WebsocketMessage & message)
        {
            const nlohmann::json pongMessage = { {"event", "pusher:pong"} };
            this->websocket.WriteMessage(pongMessage.dump());
        }

        /*
            Subscribe to pusher ping messages.
        */
        std::set<WebsocketSubscription> PusherPingEventHandler::GetSubscriptions(void) const
        {
            return {
                {
                    WebsocketSubscription::SUB_TYPE_EVENT,
                    "pusher:ping"
                }
            };
        }
    }  // namespace Websocket
}  // namespace UKControllerPlugin
