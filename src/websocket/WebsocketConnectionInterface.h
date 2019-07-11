#pragma once
#include "pch/stdafx.h"
#include "websocket/WebsocketMessage.h"

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

                virtual bool WriteMessage(std::string message) = 0;
                virtual UKControllerPlugin::Websocket::WebsocketMessage GetNextMessage(void) = 0;
        };
    }  // namespace Websocket
}  // namespace UKControllerPlugin
