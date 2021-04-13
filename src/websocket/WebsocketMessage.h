#pragma once
#include "pch/stdafx.h"

namespace UKControllerPlugin {
    namespace Websocket {


        /*
            Represents a websocket message coming into the plugin
        */
        typedef struct WebsocketMessage
        {
            // The event associated with the message
            const std::string event;

            // The channel that the message came in from
            const std::string channel;

            // The data associated with the message
            const nlohmann::json data;

            // The raw message
            const std::string raw;

            // Is the message to do with the protocol and should therefore not be processed outside the connection
            const bool protocolMessage;

            bool operator==(const WebsocketMessage & compare) const {
                return this->channel == compare.channel &&
                    this->event == compare.event &&
                    this->data == compare.data &&
                    this->protocolMessage == compare.protocolMessage;
            }
        } WebsocketMessage;
    }  // namespace Websocket
}  // namespace UKControllerPlugin
