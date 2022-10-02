#pragma once

namespace UKControllerPlugin::Integration {
    class MessageInterface;
    class Connection;

    /*
     * Represents the connection between an external integration and the plugin.
     * This may be achieved through a websocket, a winsock, or hidden window or more.
     *
     * This is used to send messages over the connection.
     */
    class IntegrationConnection
    {
        public:
        explicit IntegrationConnection(std::shared_ptr<Connection> connection);
        void Send(std::shared_ptr<MessageInterface> message) const;
        std::queue<std::shared_ptr<MessageInterface>> Receive() const;
        bool Active() const;

        private:
        // The raw connection
        std::shared_ptr<Connection> connection;
    };
} // namespace UKControllerPlugin::Integration
