#include "pch/stdafx.h"
#include "websocket/PusherActivityTimeoutEventHandler.h"

namespace UKControllerPlugin {
    namespace Websocket {

        PusherActivityTimeoutEventHandler::PusherActivityTimeoutEventHandler(WebsocketConnectionInterface & websocket)
            : websocket(websocket), defaultPingInterval(90), pongWaitInterval(30)
        {

        }

        /*
            Handles messages from the websocket
        */
        void PusherActivityTimeoutEventHandler::ProcessWebsocketMessage(const WebsocketMessage & message)
        {
            if (message.event == "pusher:pong") {
                this->pongTimeout = (std::chrono::system_clock::time_point::max)();
            } else if (message.event == "pusher:connection_established") {
                std::chrono::seconds serverPingInterval(message.data["activity_timeout"].get<int>());

                this->selectedPingInterval = serverPingInterval > this->defaultPingInterval 
                    ? serverPingInterval 
                    : this->defaultPingInterval;
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

        /*
            Every now and again, check our status
        */
        void PusherActivityTimeoutEventHandler::TimedEventTrigger(void)
        {
            // Send a ping if we've had no activity for a while
            if (this->websocket.GetTimeSinceLastActivity() > this->selectedPingInterval) {
                nlohmann::json data = {
                    {"event", "pusher:ping"}
                };

                this->websocket.WriteMessage(data.dump());
                this->pongTimeout = std::chrono::system_clock::now() + this->pongWaitInterval;
                return;
            }

            // Force a disconnect if we've waited too long for a pong
            if (this->pongTimeout > std::chrono::system_clock::now()) {
                this->websocket.ForceDisconnect();
            }
        }
    }  // namespace Websocket
}  // namespace UKControllerPlugin
