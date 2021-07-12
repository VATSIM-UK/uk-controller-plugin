#include "pch/stdafx.h"
#include "integration/IntegrationConnection.h"
#include "integration/Connection.h"
#include "integration/MessageInterface.h"
#include "integration/InboundMessage.h"

namespace UKControllerPlugin::Integration {

    IntegrationConnection::IntegrationConnection(std::shared_ptr<Connection> connection): connection(
        std::move(connection)) { }

    void IntegrationConnection::Send(std::shared_ptr<MessageInterface> message)
    {
        this->connection->Send(message->ToJson().dump());
    }

    std::shared_ptr<MessageInterface> IntegrationConnection::Receive() const
    {
        std::string receivedMessage = this->connection->Receive();
        if (receivedMessage == this->connection->NO_MESSAGE) {
            return nullptr;
        }

        try {
            return InboundMessage::FromJson(nlohmann::json::parse(receivedMessage));
        } catch (nlohmann::json::exception&) {
            LogError("Invalid JSON received from integration: " + receivedMessage);
            return nullptr;
        }
    }

    bool IntegrationConnection::Active() const
    {
        return this->connection->Active();
    }
} // namespace UKControllerPlugin::Integration
