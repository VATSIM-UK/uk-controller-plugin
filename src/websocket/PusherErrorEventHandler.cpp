#include "pch/stdafx.h"
#include "websocket/PusherErrorEventHandler.h"

using UKControllerPlugin::Windows::WinApiInterface;

namespace UKControllerPlugin {
    namespace Websocket {

        void PusherErrorEventHandler::ProcessWebsocketMessage(const WebsocketMessage & message)
        {
            LogError("Websocket error, message: " + message.data["message"].get<std::string>());
        }

        /*
            Which events to subscribe to
        */
        std::set<WebsocketSubscription> PusherErrorEventHandler::GetSubscriptions(void) const
        {
            return {
                {
                    WebsocketSubscription::SUB_TYPE_EVENT,
                    "pusher:error"
                }
            };
        }
    }  // namespace Websocket
}  // namespace UKControllerPlugin
