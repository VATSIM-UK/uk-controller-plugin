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

        private:
            bool AttemptInitialisation(
                std::shared_ptr<IntegrationConnection> connection,
                std::queue<std::shared_ptr<MessageInterface>> incomingMessages
            );
            static bool InitialisationMessageValid(std::shared_ptr<MessageInterface> message);
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
