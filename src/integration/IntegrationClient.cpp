#include "pch/stdafx.h"
#include "integration/IntegrationClient.h"
#include "integration/IntegrationConnection.h"
#include "integration/InboundMessage.h"
#include "integration/MessageType.h"

namespace UKControllerPlugin::Integration {

    const std::set<const std::shared_ptr<MessageType>>& IntegrationClient::InterestedMessages() const
    {
        return this->interestedMessages;
    }

    bool IntegrationClient::InterestedInMessage(const MessageType& message) const
    {
        return std::find_if(
            this->interestedMessages.cbegin(),
            this->interestedMessages.cend(),
            [&message](const std::shared_ptr<MessageType>& interestedMessage) -> bool
            {
                return message == *interestedMessage;
            }
        ) != this->interestedMessages.cend();
    }

    void IntegrationClient::ClearInterestedMessages()
    {
        this->interestedMessages.clear();
    }

    void IntegrationClient::AddInterestedMessage(std::shared_ptr<MessageType> message)
    {
        this->interestedMessages.insert(message);
    }

    void IntegrationClient::Send(const std::shared_ptr<MessageInterface> message) const
    {
        auto messageData = message->GetMessageType().ToJson();
        messageData.update(message->GetMessageData());
        this->connection->Send(messageData.dump());
    }

    bool IntegrationClient::Active() const
    {
        return this->connection->Active();
    }

    std::string IntegrationClient::GetIntegrationString() const
    {
        return this->integrationName + "/" + this->integrationVersion;
    }

    bool IntegrationClient::MessageFormatValid(const nlohmann::json& message)
    {
        return message.contains("type") &&
            message.at("type").is_string() &&
            message.contains("version") &&
            message.at("version").is_number_integer() &&
            message.contains("data") &&
            message.at("data").is_object();
    }

    std::shared_ptr<MessageInterface> IntegrationClient::Receive()
    {
        auto nextMessage = this->connection->Receive();
        if (nextMessage == this->connection->NO_MESSAGES_PENDING) {
            return nullptr;
        }

        try {
            nlohmann::json messageData = nlohmann::json::parse(nextMessage);
            if (!MessageFormatValid(messageData)) {
                LogError("Invalid message from integration " + this->GetIntegrationString() + ": " + nextMessage);
                return nullptr;
            }

            return std::make_shared<InboundMessage>(
                MessageType{
                    messageData.at("type").get<std::string>(),
                    messageData.at("version").get<int>()
                },
                messageData.at("data")
            );
        } catch (nlohmann::json::exception&) {
            LogError("Bad JSON from integration " + this->GetIntegrationString() + ": " + nextMessage);
            return nullptr;
        }
    }
} // namespace UKControllerPlugin::Integration
