#pragma once
#include "pch/stdafx.h"

namespace UKControllerPlugin::Integration {

    /*
     * Represents the connection between an external integration and the plugin.
     * This may be achieved through a websocket, a winsock, or hidden window or more.
     *
     * This is used to send messages over the connection.
     */
    class Connection
    {
        public:
            virtual ~Connection() = default;

            /*
             * Send a single message over the connection.
             */
            virtual void Send(std::string message) = 0;

            /*
             * Receive a sequence of queued messages.
             */
            virtual std::queue<std::string> Receive() = 0;

            /*
             * Is the connection still active
             */
            virtual bool Active() const = 0;
    };
} // namespace UKControllerPlugin::Integration
