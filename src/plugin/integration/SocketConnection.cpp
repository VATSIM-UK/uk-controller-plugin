#include "SocketConnection.h"
#include "SocketInterface.h"

namespace UKControllerPlugin::Integration {
    SocketConnection::SocketConnection(std::shared_ptr<SocketInterface> socket) : socket(std::move(socket))
    {
    }

    auto SocketConnection::Active() const -> bool
    {
        return this->socket->Active();
    }

    auto SocketConnection::Receive() -> std::queue<std::string>
    {
        *this->socket >> this->incomingData;
        return this->ProcessReceivedData();
    }

    void SocketConnection::Send(std::string message)
    {
        message.append(MESSAGE_DELIMITER);
        *this->socket << message;
    }

    auto SocketConnection::ProcessReceivedData() -> std::queue<std::string>
    {
        if (this->incomingData.tellp() == 0) {
            return {};
        }

        std::string streamData = this->incomingData.str();
        std::queue<std::string> messages;

        size_t indexOfMessageSeparator = 0;
        while ((indexOfMessageSeparator = streamData.find(MESSAGE_DELIMITER)) != std::string::npos) {
            messages.push(streamData.substr(0, indexOfMessageSeparator));
            streamData.erase(0, indexOfMessageSeparator + 1);
        }

        // Put any incomplete messages back on the stream for later
        this->incomingData.str("");
        this->incomingData.clear();
        this->incomingData << streamData;

        return messages;
    }
} // namespace UKControllerPlugin::Integration
