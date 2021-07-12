#pragma once
#include "integration/Connection.h"

namespace UKControllerPlugin::Integration {

    /*
     * A connection for integrations via a socket
     */
    class SocketConnection : public Connection
    {
        public:
            SocketConnection(SOCKET socket);
            ~SocketConnection() override;

            bool Active() const override;
            std::string Receive() override;
            void Send(std::string message) override;

        private:
            std::lock_guard<std::mutex> LockQueue();
            void ReceiveLoop();
            void ProcessReceivedData(std::vector<char>& data);

            static inline const char MESSAGE_DELIMITER = '\0x1f';

            // The socket
            SOCKET socket;

            // Are we active
            bool active = true;

            // Protects the message queue
            std::mutex queueLock;

            // Data that is inbound, but not yet part of a message
            std::stringstream incomingData;

            // The inbound message receiving thread
            std::shared_ptr<std::thread> receiveThread;

            // Are we expecting some length
            bool expectingLength = true;

            // Messages
            std::queue<std::string> messages;
    };
} // namespace UKControllerPlugin::Integration
