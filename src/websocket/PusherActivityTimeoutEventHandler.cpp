#include "pch/stdafx.h"
#include "websocket/PusherActivityTimeoutEventHandler.h"

namespace UKControllerPlugin {
    namespace Websocket {

        PusherActivityTimeoutEventHandler::PusherActivityTimeoutEventHandler(WebsocketConnectionInterface & websocket)
            : websocket(websocket), defaultTimeoutInterval(60)
        {

        }

        /*
            Handles messages from the websocket
        */
        void PusherActivityTimeoutEventHandler::ProcessWebsocketMessage(const WebsocketMessage & message)
        {
            if (message.event == "pusher:connection_established") {
                std::chrono::seconds serverPingInterval(message.data["activity_timeout"].get<int>());

                // Boost starts pinging after half the interval, so we want to double whatever the server sends
                std::chrono::seconds timeoutInterval = serverPingInterval * 2 < this->defaultTimeoutInterval
                    ? serverPingInterval * 2
                    : this->defaultTimeoutInterval;

                this->websocket.SetIdleTimeout(timeoutInterval);
            }
        }

        /*
            Get the event subscriptions
        */
        std::set<WebsocketSubscription> PusherActivityTimeoutEventHandler::GetSubscriptions(void) const
        {
            return {
                {
                    WebsocketSubscription::SUB_TYPE_EVENT,
                    "pusher:connection_established"
                },
                {
                    WebsocketSubscription::SUB_TYPE_EVENT,
                    "pusher:pong"
                }
            };
        }
    }  // namespace Websocket
}  // namespace UKControllerPlugin
