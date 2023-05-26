#include "ClientInitialisationManager.h"
#include "InitialisationFailureMessage.h"
#include "InitialisationSuccessMessage.h"
#include "IntegrationClient.h"
#include "IntegrationClientManager.h"
#include "IntegrationConnection.h"
#include "IntegrationDataInitialisers.h"
#include "time/SystemClock.h"

namespace UKControllerPlugin::Integration {

    ClientInitialisationManager::ClientInitialisationManager(
        std::shared_ptr<IntegrationClientManager> clientManager,
        std::shared_ptr<IntegrationDataInitialisers> dataInitialisers)
        : clientManager(std::move(clientManager)), dataInitialisers(std::move(dataInitialisers))
    {
        assert(this->clientManager && "Client manager not set in ClientInitialisationManager");
        assert(this->dataInitialisers && "Data initialisers not set in ClientInitialisationManager");
    }

    void ClientInitialisationManager::AddConnection(std::shared_ptr<IntegrationConnection> connection)
    {
        if (this->connections.contains(connection)) {
            LogWarning("A duplicate integration client was added");
            return;
        }

        this->connections[std::move(connection)] = Time::TimeNow();
    }

    void ClientInitialisationManager::TimedEventTrigger()
    {
        for (auto connection = this->connections.begin(); connection != this->connections.end();) {
            // If it's taken too long to initialise, then kill the connection
            if (Time::TimeNow() > connection->second + INITIALISATION_TIMEOUT) {
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
                if (incomingMessages.size() > 1) {
                    LogWarning("Dropped some messages received before initialisation completed");
                }
                this->connections.erase(connection++);
            } else {
                ++connection;
            }
        }
    }

    auto ClientInitialisationManager::CountConnections() const -> size_t
    {
        return this->connections.size();
    }

    auto ClientInitialisationManager::AttemptInitialisation(
        const std::shared_ptr<IntegrationConnection>& connection,
        std::queue<std::shared_ptr<MessageInterface>> incomingMessages) -> bool
    {
        while (!incomingMessages.empty()) {
            auto validationErrors = ValidateMessage(incomingMessages.front());
            if (!validationErrors.empty()) {
                LogError("Invalid integration initialisation message: " + incomingMessages.front()->ToJson().dump());
                SendInitialisationFailureMessage(connection, incomingMessages.front(), validationErrors);
                incomingMessages.pop();
                continue;
            }

            const auto client = this->UpgradeToClient(connection, incomingMessages.front());
            SendInitialisationSuccessMessage(connection, incomingMessages.front());
            this->dataInitialisers->InitialiseClient(*client);

            return true;
        }

        return false;
    }

    auto ClientInitialisationManager::ValidateMessage(const std::shared_ptr<MessageInterface>& message)
        -> std::vector<std::string>
    {
        auto errors = ValidateMessageType(message);
        auto dataErrors = ValidateMessageData(message);
        errors.insert(
            errors.end(), std::make_move_iterator(dataErrors.cbegin()), std::make_move_iterator(dataErrors.cend()));

        return errors;
    }

    auto ClientInitialisationManager::ValidateMessageType(const std::shared_ptr<MessageInterface>& message)
        -> std::vector<std::string>
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

    auto ClientInitialisationManager::ValidateMessageData(const std::shared_ptr<MessageInterface>& message)
        -> std::vector<std::string>
    {
        auto messageData = message->GetMessageData();
        std::vector<std::string> errors = ValidateIntegrationDetails(messageData);
        auto eventSubscriptionErrors = ValidateEventSubscriptions(messageData);
        errors.insert(
            errors.end(),
            std::make_move_iterator(eventSubscriptionErrors.cbegin()),
            std::make_move_iterator(eventSubscriptionErrors.cend()));
        return errors;
    }

    auto ClientInitialisationManager::ValidateIntegrationDetails(const nlohmann::json& data) -> std::vector<std::string>
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

    auto ClientInitialisationManager::ValidateEventSubscriptions(const nlohmann::json& data) -> std::vector<std::string>
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

    auto ClientInitialisationManager::UpgradeToClient(
        const std::shared_ptr<IntegrationConnection>& connection,
        const std::shared_ptr<MessageInterface>& initialisationMessage) -> std::shared_ptr<IntegrationClient>
    {
        auto data = initialisationMessage->GetMessageData();
        auto client = std::make_shared<IntegrationClient>(
            this->nextIntegrationId++,
            data.at("integration_name").get<std::string>(),
            data.at("integration_version").get<std::string>(),
            connection);
        for (const auto& interestedEvent : data.at("event_subscriptions")) {
            client->AddInterestedMessage(std::make_shared<MessageType>(
                MessageType{interestedEvent.at("type").get<std::string>(), interestedEvent.at("version").get<int>()}));
        }
        this->clientManager->AddClient(client);

        return client;
    }

    void ClientInitialisationManager::SendInitialisationSuccessMessage(
        const std::shared_ptr<IntegrationConnection>& connection,
        const std::shared_ptr<MessageInterface>& initialisationMessage)
    {
        connection->Send(std::make_shared<InitialisationSuccessMessage>(initialisationMessage->GetMessageId()));
    }

    void ClientInitialisationManager::SendInitialisationFailureMessage(
        const std::shared_ptr<IntegrationConnection>& connection,
        const std::shared_ptr<MessageInterface>& initialisationMessage,
        const std::vector<std::string>& errors)
    {
        connection->Send(std::make_shared<InitialisationFailureMessage>(initialisationMessage->GetMessageId(), errors));
    }
} // namespace UKControllerPlugin::Integration
