#include "pch/stdafx.h"
#include "integration/ClientInitialisationManager.h"
#include "integration/InitialistionSuccessMessage.h"
#include "integration/IntegrationClient.h"
#include "integration/MessageInterface.h"
#include "integration/IntegrationConnection.h"
#include "integration/IntegrationClientManager.h"
#include "time/SystemClock.h"

namespace UKControllerPlugin::Integration {

    ClientInitialisationManager::ClientInitialisationManager(std::shared_ptr<IntegrationClientManager> clientManager):
        clientManager(std::move(clientManager)) {}

    void ClientInitialisationManager::AddConnection(std::shared_ptr<IntegrationConnection> connection)
    {
        if (this->connections.count(connection)) {
            LogWarning("A duplicate integration client was added");
            return;
        }

        this->connections[connection] = Time::TimeNow();
    }

    void ClientInitialisationManager::TimedEventTrigger()
    {
        for (
            auto connection = this->connections.begin();
            connection != this->connections.end();
        ) {
            // If it's taken too long to initialise, then kill the connection
            if (Time::TimeNow() > connection->second + std::chrono::seconds(10)) {
                LogInfo("Integration has not initialised in time");
                this->connections.erase(connection++);
                continue;
            }

            // Check for an init message and if so, elevate to client level, ignore anything else
            auto incomingMessages = connection->first->Receive();
            if (incomingMessages.empty()) {
                ++connection;
                continue;
            }

            if (this->AttemptInitialisation(connection->first, incomingMessages)) {
                this->connections.erase(connection++);
            } else {
                ++connection;
            }
        }
    }

    size_t ClientInitialisationManager::CountConnections() const
    {
        return this->connections.size();
    }

    bool ClientInitialisationManager::AttemptInitialisation(
        std::shared_ptr<IntegrationConnection> connection,
        std::queue<std::shared_ptr<MessageInterface>> incomingMessages
    )
    {
        while (!incomingMessages.empty()) {
            if (!InitialisationMessageValid(incomingMessages.front())) {
                LogError(
                    "Invalid integration initialisation message: " + incomingMessages.front()->ToJson().dump()
                );
                incomingMessages.pop();
                continue;
            }

            this->UpgradeToClient(connection, incomingMessages.front());
            this->SendInitialisationSuccessMessage(connection);
            return true;
        }

        return false;
    }

    bool ClientInitialisationManager::InitialisationMessageValid(std::shared_ptr<MessageInterface> message)
    {
        auto messageType = message->GetMessageType();
        auto messageData = message->GetMessageData();

        return messageType.type == "initialise" &&
            messageType.version == 1 &&
            messageData.contains("integration_name") &&
            messageData.at("integration_name").is_string() &&
            messageData.contains("integration_version") &&
            messageData.at("integration_version").is_string() &&
            messageData.contains("event_subscriptions") &&
            messageData.at("event_subscriptions").is_array() &&
            std::find_if_not(
                messageData.at("event_subscriptions").cbegin(),
                messageData.at("event_subscriptions").cend(),
                [](const nlohmann::json& subscription) -> bool
                {
                    return subscription.is_object() &&
                        subscription.contains("type") &&
                        subscription.at("type").is_string() &&
                        subscription.contains("version") &&
                        subscription.at("version").is_number_integer();
                }
            ) == messageData.at("event_subscriptions").cend();
    }

    void ClientInitialisationManager::UpgradeToClient(
        std::shared_ptr<IntegrationConnection> connection,
        std::shared_ptr<MessageInterface> initialisationMessage
    )
    {
        auto data = initialisationMessage->GetMessageData();
        auto client = std::make_shared<IntegrationClient>(
            this->nextIntegrationId++,
            data.at("integration_name").get<std::string>(),
            data.at("integration_version").get<std::string>(),
            connection
        );
        for (const auto& interestedEvent : data.at("event_subscriptions")) {
            client->AddInterestedMessage(
                std::make_shared<MessageType>(MessageType{
                    interestedEvent.at("type").get<std::string>(),
                    interestedEvent.at("version").get<int>()
                })
            );
        }
        this->clientManager->AddClient(client);
    }

    void ClientInitialisationManager::SendInitialisationSuccessMessage(
        std::shared_ptr<IntegrationConnection> connection
    ) const
    {
        connection->Send(std::make_shared<InitialisationSuccessMessage>());
    }
} // namespace UKControllerPlugin::Integration
