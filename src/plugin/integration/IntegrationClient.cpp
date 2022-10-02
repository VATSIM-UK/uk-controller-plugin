#include "integration/IntegrationClient.h"
#include "integration/IntegrationConnection.h"
#include "integration/MessageType.h"

namespace UKControllerPlugin::Integration {
    IntegrationClient::IntegrationClient(
        int id,
        std::string integrationName,
        std::string integrationVersion,
        std::shared_ptr<IntegrationConnection> connection)
        : id(id), integrationName(std::move(integrationName)), integrationVersion(std::move(integrationVersion)),
          connection(std::move(connection))
    {
        LogInfo("Initialised integration: " + this->GetIntegrationString());
    }

    IntegrationClient::~IntegrationClient()
    {
        LogInfo("Shutting down integration: " + this->GetIntegrationString());
    }

    const std::set<std::shared_ptr<MessageType>>& IntegrationClient::InterestedMessages() const
    {
        return this->interestedMessages;
    }

    bool IntegrationClient::InterestedInMessage(const MessageType& message) const
    {
        return std::find_if(
                   this->interestedMessages.cbegin(),
                   this->interestedMessages.cend(),
                   [&message](const std::shared_ptr<MessageType>& interestedMessage) -> bool {
                       return message == *interestedMessage;
                   }) != this->interestedMessages.cend();
    }

    void IntegrationClient::ClearInterestedMessages()
    {
        this->interestedMessages.clear();
    }

    void IntegrationClient::AddInterestedMessage(std::shared_ptr<MessageType> message)
    {
        this->interestedMessages.insert(message);
    }

    const std::shared_ptr<IntegrationConnection> IntegrationClient::Connection() const
    {
        return this->connection;
    }

    int IntegrationClient::Id() const
    {
        return this->id;
    }

    std::string IntegrationClient::GetIntegrationString() const
    {
        return this->integrationName + "/" + this->integrationVersion;
    }
} // namespace UKControllerPlugin::Integration
