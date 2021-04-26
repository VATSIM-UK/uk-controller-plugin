#pragma once
#include "pch/stdafx.h"

namespace UKControllerPlugin {
    namespace Websocket {

        /*
            Class that represents a websocket connection
            to the web API. The web API operates a read-only
            websocket connection.
        */
        class WebsocketConnectionInterface
        {
            public:

                virtual ~WebsocketConnectionInterface(void) {}
                virtual void WriteMessage(std::string message) = 0;
                virtual std::string GetNextMessage(void) = 0;
                virtual void SetIdleTimeout(std::chrono::seconds timeout) = 0;

                // Returned when there are no messages in the queue to be processed
                const std::string noMessage = "";
        };
    }  // namespace Websocket
}  // namespace UKControllerPlugin
