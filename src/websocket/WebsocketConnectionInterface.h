#pragma once
#include "pch/stdafx.h"
#include "websocket/WebsocketMessage.h"

namespace UKControllerPlugin {
    namespace Websocket {

        /*
            Class that represents a websocket connection
            to the web API.
        */
        class WebsocketConnectionInterface
        {
            public:

                virtual bool WriteMessage(std::string message) = 0;
                virtual UKControllerPlugin::Websocket::WebsocketMessage GetNextMessage(void) = 0;
        };
    }  // namespace Websocket
}  // namespace UKControllerPlugin
