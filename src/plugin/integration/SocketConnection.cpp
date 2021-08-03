#include "pch/pch.h"
#include "integration/SocketConnection.h"
#include "integration/SocketInterface.h"

namespace UKControllerPlugin::Integration {
    SocketConnection::SocketConnection(std::shared_ptr<SocketInterface> socket): socket(socket)
    { }

    bool SocketConnection::Active() const
    {
        return this->socket->Active();
    }

    std::queue<std::string> SocketConnection::Receive()
    {
        this->incomingData.clear();
        *this->socket >> this->incomingData;
        return this->ProcessReceivedData();
    }

    void SocketConnection::Send(std::string message)
    {
        message.append(MESSAGE_DELIMITER);
        *this->socket << message;
    }

    std::queue<std::string> SocketConnection::ProcessReceivedData()
    {
        if (this->incomingData.tellp() == 0) {
            return {};
        }

        std::string streamData;
        std::queue<std::string> messages;
        this->incomingData >> streamData;

        size_t indexOfMessageSeparator;
        while ((indexOfMessageSeparator = streamData.find(MESSAGE_DELIMITER)) != std::string::npos) {
            messages.push(streamData.substr(0, indexOfMessageSeparator));
            streamData.erase(0, indexOfMessageSeparator + 1);
        }

        // Put any incomplete messages back on the stream for later
        if (!streamData.empty()) {
            this->incomingData = std::stringstream();
            this->incomingData << streamData;
        }

        return messages;
    }
} // namespace UKControllerPlugin::Integration
