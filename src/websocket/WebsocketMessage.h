#pragma once
#include "pch/stdafx.h"

namespace UKControllerPlugin {
    namespace Websocket {


        typedef struct WebsocketMessage
        {
            // The channel that the message came in from
            const std::string channel;

            // The content of the message
            const std::string content;
        } WebsocketMessage;
    }  // namespace Websocket
}  // namespace UKControllerPlugin
