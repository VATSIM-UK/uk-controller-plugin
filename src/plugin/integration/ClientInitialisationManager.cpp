#include "pch/pch.h"
#include "integration/ClientInitialisationManager.h"
#include "integration/InitialisationSuccessMessage.h"
#include "integration/InitialisationFailureMessage.h"
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

        this->connections[std::move(connection)] = Time::TimeNow();
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
        const std::shared_ptr<IntegrationConnection>& connection,
        std::queue<std::shared_ptr<MessageInterface>> incomingMessages
    )
    {
        while (!incomingMessages.empty()) {
            auto validationErrors = ValidateMessage(incomingMessages.front());
            if (!validationErrors.empty()) {
                LogError(
                    "Invalid integration initialisation message: " + incomingMessages.front()->ToJson().dump()
                );
                SendInitialisationFailureMessage(connection, incomingMessages.front(), validationErrors);
                incomingMessages.pop();
                continue;
            }

            this->UpgradeToClient(connection, incomingMessages.front());
            SendInitialisationSuccessMessage(connection, incomingMessages.front());
            return true;
        }

        return false;
    }

    std::vector<std::string> ClientInitialisationManager::ValidateMessage(
        const std::shared_ptr<MessageInterface>& message
    )
    {
        auto errors = ValidateMessageType(message);
        auto dataErrors = ValidateMessageData(message);
        errors.insert(
            errors.end(),
            std::make_move_iterator(dataErrors.cbegin()),
            std::make_move_iterator(dataErrors.cend())
        );

        return errors;
    }

    std::vector<std::string> ClientInitialisationManager::ValidateMessageType(
        const std::shared_ptr<MessageInterface>& message
    )
    {
        std::vector<std::string> errors;
        auto messageType = message->GetMessageType();
        if (messageType.type != "initialise") {
            errors.push_back(VALIDATION_ERROR_INVALID_TYPE);
        }

        if (messageType.version != 1) {
            errors.push_back(VALIDATION_ERROR_INVALID_VERSION);
        }

        return errors;
    }

    std::vector<std::string> ClientInitialisationManager::ValidateMessageData(
        const std::shared_ptr<MessageInterface>& message
    )
    {
        auto messageData = message->GetMessageData();
        std::vector<std::string> errors = ValidateIntegrationDetails(messageData);
        auto eventSubscriptionErrors = ValidateEventSubscriptions(messageData);
        errors.insert(
            errors.end(),
            std::make_move_iterator(eventSubscriptionErrors.cbegin()),
            std::make_move_iterator(eventSubscriptionErrors.cend())
        );
        return errors;
    }

    std::vector<std::string> ClientInitialisationManager::ValidateIntegrationDetails(const nlohmann::json& data)
    {
        std::vector<std::string> errors;
        if (!data.contains("integration_name") || !data.at("integration_name").is_string()) {
            errors.push_back(VALIDATION_ERROR_INVALID_INTEGRATION_NAME);
        }

        if (!data.contains("integration_version") || !data.at("integration_version").is_string()) {
            errors.push_back(VALIDATION_ERROR_INVALID_INTEGRATION_VERSION);
        }

        return errors;
    }

    std::vector<std::string> ClientInitialisationManager::ValidateEventSubscriptions(const nlohmann::json& data)
    {
        std::vector<std::string> errors;
        if (!data.contains("event_subscriptions") || !data.at("event_subscriptions").is_array()) {
            errors.push_back(VALIDATION_ERROR_INVALID_SUBSCRIPTIONS);
            return errors;
        }

        for (const auto& subscription : data.at("event_subscriptions")) {
            if (!subscription.is_object()) {
                errors.push_back(VALIDATION_ERROR_INVALID_SUBSCRIPTION);
                continue;
            }

            if (!subscription.contains("type") || !subscription.at("type").is_string()) {
                errors.push_back(VALIDATION_ERROR_INVALID_SUBSCRIPTION_TYPE);
            }

            if (!subscription.contains("version") || !subscription.at("version").is_number_integer()) {
                errors.push_back(VALIDATION_ERROR_INVALID_SUBSCRIPTION_VERSION);
            }
        }

        return errors;
    }

    void ClientInitialisationManager::UpgradeToClient(
        const std::shared_ptr<IntegrationConnection>& connection,
        const std::shared_ptr<MessageInterface>& initialisationMessage
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
        const std::shared_ptr<IntegrationConnection>& connection,
        const std::shared_ptr<MessageInterface>& initialisationMessage
    ) {
        connection->Send(std::make_shared<InitialisationSuccessMessage>(initialisationMessage->GetMessageId()));
    }
    
    void ClientInitialisationManager::SendInitialisationFailureMessage(
        const std::shared_ptr<IntegrationConnection>& connection,
        const std::shared_ptr<MessageInterface>& initialisationMessage,
        const std::vector<std::string>& errors
    ) {
        connection->Send(std::make_shared<InitialisationFailureMessage>(
            initialisationMessage->GetMessageId(),
            errors
        ));
    }
} // namespace UKControllerPlugin::Integration
