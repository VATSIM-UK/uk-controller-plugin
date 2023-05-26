#pragma once
#include "timedevent/AbstractTimedEvent.h"

namespace UKControllerPlugin::Integration {
    class IntegrationConnection;
    class IntegrationClient;
    class IntegrationClientManager;
    class IntegrationDataInitialisers;
    class MessageInterface;

    /*
     * Manages the initialisation of clients and
     * ensures that clients are properly initialised before being
     * allowed to interact with the plugin.
     */
    class ClientInitialisationManager : public TimedEvent::AbstractTimedEvent
    {
        public:
        explicit ClientInitialisationManager(
            std::shared_ptr<IntegrationClientManager> clientManager,
            std::shared_ptr<IntegrationDataInitialisers> dataInitialisers);
        void AddConnection(std::shared_ptr<IntegrationConnection> connection);
        void TimedEventTrigger() override;
        [[nodiscard]] auto CountConnections() const -> size_t;

        const inline static std::string VALIDATION_ERROR_INVALID_TYPE =
            "Initialisation messages must have type \"initialise\"";

        const inline static std::string VALIDATION_ERROR_INVALID_VERSION =
            "Initialisation messages must have version 1";

        const inline static std::string VALIDATION_ERROR_INVALID_INTEGRATION_NAME = "Invalid integration name";

        const inline static std::string VALIDATION_ERROR_INVALID_INTEGRATION_VERSION = "Invalid integration version";

        const inline static std::string VALIDATION_ERROR_INVALID_SUBSCRIPTIONS =
            "Expected array of integration subscription";

        const inline static std::string VALIDATION_ERROR_INVALID_SUBSCRIPTION =
            "Invalid subscription - must be an object";

        const inline static std::string VALIDATION_ERROR_INVALID_SUBSCRIPTION_TYPE = "Invalid subscription type";

        const inline static std::string VALIDATION_ERROR_INVALID_SUBSCRIPTION_VERSION = "Invalid subscription version";

        private:
        auto AttemptInitialisation(
            const std::shared_ptr<IntegrationConnection>& connection,
            std::queue<std::shared_ptr<MessageInterface>> incomingMessages) -> bool;
        static auto ValidateMessage(const std::shared_ptr<MessageInterface>& message) -> std::vector<std::string>;
        static auto ValidateMessageType(const std::shared_ptr<MessageInterface>& message) -> std::vector<std::string>;
        static auto ValidateMessageData(const std::shared_ptr<MessageInterface>& message) -> std::vector<std::string>;
        static auto ValidateIntegrationDetails(const nlohmann::json& data) -> std::vector<std::string>;
        static auto ValidateEventSubscriptions(const nlohmann::json& data) -> std::vector<std::string>;
        [[nodiscard]] auto UpgradeToClient(
            const std::shared_ptr<IntegrationConnection>& connection,
            const std::shared_ptr<MessageInterface>& initialisationMessage) -> std::shared_ptr<IntegrationClient>;
        static void SendInitialisationSuccessMessage(
            const std::shared_ptr<IntegrationConnection>& connection,
            const std::shared_ptr<MessageInterface>& initialisationMessage);
        static void SendInitialisationFailureMessage(
            const std::shared_ptr<IntegrationConnection>& connection,
            const std::shared_ptr<MessageInterface>& initialisationMessage,
            const std::vector<std::string>& errors);

        // Manages clients once fully initialised
        std::shared_ptr<IntegrationClientManager> clientManager;

        // Handles data initialisation
        std::shared_ptr<IntegrationDataInitialisers> dataInitialisers;

        // Clients that are fully initialised
        std::map<std::shared_ptr<IntegrationConnection>, std::chrono::system_clock::time_point> connections;

        // The next unique id for the integration
        int nextIntegrationId = 1;

        // How long an integration has to initialise before we drop the connection
        const std::chrono::seconds INITIALISATION_TIMEOUT = std::chrono::seconds(10);
    };
} // namespace UKControllerPlugin::Integration
