#pragma once
#include "timedevent/AbstractTimedEvent.h"

namespace UKControllerPlugin::Integration {
    class IntegrationConnection;
    class IntegrationClientManager;
    class MessageInterface;

    /*
     * Manages the initialisation of clients and
     * ensures that clients are properly initialised before being
     * allowed to interact with the plugin.
     */
    class ClientInitialisationManager : public TimedEvent::AbstractTimedEvent
    {
        public:
            explicit ClientInitialisationManager(std::shared_ptr<IntegrationClientManager> clientManager);
            ~ClientInitialisationManager() override = default;
            void AddConnection(std::shared_ptr<IntegrationConnection> connection);
            void TimedEventTrigger() override;
            size_t CountConnections() const;

            const inline static std::string VALIDATION_ERROR_INVALID_TYPE =
                "Initialisation messages must have type \"initialise\"";

            const inline static std::string VALIDATION_ERROR_INVALID_VERSION =
                "Initialisation messages must have version 1";

            const inline static std::string VALIDATION_ERROR_INVALID_INTEGRATION_NAME =
                "Invalid integration name";

            const inline static std::string VALIDATION_ERROR_INVALID_INTEGRATION_VERSION =
                "Invalid integration version";

            const inline static std::string VALIDATION_ERROR_INVALID_SUBSCRIPTIONS =
                "Expected array of integration subscription";

            const inline static std::string VALIDATION_ERROR_INVALID_SUBSCRIPTION =
                "Invalid subscription - must be an object";

            const inline static std::string VALIDATION_ERROR_INVALID_SUBSCRIPTION_TYPE =
                "Invalid subscription type";

            const inline static std::string VALIDATION_ERROR_INVALID_SUBSCRIPTION_VERSION =
                "Invalid subscription version";

        private:
            bool AttemptInitialisation(
                std::shared_ptr<IntegrationConnection> connection,
                std::queue<std::shared_ptr<MessageInterface>> incomingMessages
            );
            static std::vector<std::string> ValidateMessage(std::shared_ptr<MessageInterface> message);
            static std::vector<std::string> ValidateMessageType(std::shared_ptr<MessageInterface> message);
            static std::vector<std::string> ValidateMessageData(std::shared_ptr<MessageInterface> message);
            static std::vector<std::string> ValidateIntegrationDetails(const nlohmann::json& data);
            static std::vector<std::string> ValidateEventSubscriptions(const nlohmann::json& data);
            void UpgradeToClient(
                std::shared_ptr<IntegrationConnection> connection,
                std::shared_ptr<MessageInterface> initialisationMessage
            );
            void SendInitialisationSuccessMessage(std::shared_ptr<IntegrationConnection> connection) const;

            // Manages clients once fully initialised
            std::shared_ptr<IntegrationClientManager> clientManager;

            // Clients that are fully initialised
            std::map<std::shared_ptr<IntegrationConnection>, std::chrono::system_clock::time_point> connections;

            // The next unique id for the integration
            int nextIntegrationId = 1;
    };
} // namespace UKControllerPlugin::Integration
