#pragma once

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
        [[nodiscard]] virtual auto Receive() -> std::queue<std::string> = 0;

        /*
         * Is the connection still active
         */
        [[nodiscard]] virtual auto Active() const -> bool = 0;
    };
} // namespace UKControllerPlugin::Integration
