#include "pch/pch.h"
#include "integration/IntegrationConnection.h"
#include "integration/Connection.h"
#include "integration/MessageInterface.h"
#include "integration/InboundMessage.h"

namespace UKControllerPlugin::Integration {

    IntegrationConnection::IntegrationConnection(std::shared_ptr<Connection> connection): connection(
        std::move(connection))
    {
        LogInfo("Initialising new integration connection");
    }

    void IntegrationConnection::Send(std::shared_ptr<MessageInterface> message) const
    {
        this->connection->Send(message->ToJson().dump());
    }

    std::queue<std::shared_ptr<MessageInterface>> IntegrationConnection::Receive() const
    {
        std::queue<std::string> messages = this->connection->Receive();
        std::queue<std::shared_ptr<MessageInterface>> parsedMessages;
        while (!messages.empty()) {
            try {
                auto message = InboundMessage::FromJson(nlohmann::json::parse(messages.front()));
                if (!message) {
                    LogError("Invalid message received from integration: " + messages.front());
                } else {
                    parsedMessages.push(message);
                }
            } catch (nlohmann::json::exception&) {
                LogError("Invalid JSON received from integration: " + messages.front());
            }

            messages.pop();
        }

        return parsedMessages;
    }

    bool IntegrationConnection::Active() const
    {
        return this->connection->Active();
    }
} // namespace UKControllerPlugin::Integration
