#include "pch/stdafx.h"
#include "websocket/PollingWebsocketConnection.h"

namespace UKControllerPlugin {
    namespace Websocket {

        PollingWebsocketConnection::PollingWebsocketConnection(const Api::ApiInterface& api): api(api) { }

        void PollingWebsocketConnection::WriteMessage(std::string message)
        {
            // Nothing to do here
        }

        std::string PollingWebsocketConnection::GetNextMessage()
        {
            return std::string();
        }

        void PollingWebsocketConnection::SetIdleTimeout(std::chrono::seconds timeout)
        {
            // Nothing to do here
        }
    }  // namespace Websocket
}  // namespace UKControllerPlugin
