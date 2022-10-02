#pragma once
#include "integration/Connection.h"

namespace UKControllerPlugin::Integration {
    class MessageInterface;
    class SocketInterface;

    /*
     * A connection for integrations via a socket
     */
    class SocketConnection : public Connection
    {
        public:
        explicit SocketConnection(std::shared_ptr<SocketInterface> socket);
        ~SocketConnection() override = default;

        bool Active() const override;
        std::queue<std::string> Receive() override;
        void Send(std::string message) override;

        private:
        std::queue<std::string> ProcessReceivedData();

        static inline const char MESSAGE_DELIMITER[1] = {'\x1F'};

        // The socket
        std::shared_ptr<SocketInterface> socket;

        // Data that is inbound, but not yet part of a message
        std::stringstream incomingData;
    };
} // namespace UKControllerPlugin::Integration
