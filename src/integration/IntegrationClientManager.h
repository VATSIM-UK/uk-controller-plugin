#pragma once
#include "timedevent/AbstractTimedEvent.h"

namespace UKControllerPlugin::Integration {
    class IntegrationClient;
    class IntegrationConnection;
    class MessageInterface;

    /*
     * Manages the lifetime of and messages to
     * integration clients.
     */
    class IntegrationClientManager : public TimedEvent::AbstractTimedEvent
    {
        public:
            IntegrationClientManager() = default;
            ~IntegrationClientManager();
            void AddClient(std::shared_ptr<IntegrationClient> client);
            void SendMessageToInterestedClients(std::shared_ptr<MessageInterface> message);
            void TimedEventTrigger() override;
            std::shared_ptr<IntegrationClient> GetById(int id) const;
            using IntegrationClients = std::set<std::shared_ptr<IntegrationClient>>;
            using const_iterator = IntegrationClients::const_iterator;
            const_iterator cbegin() const { return clients.cbegin(); }
            const_iterator cend() const { return clients.cend(); }
            size_t CountClients() const;

        private:
            void RemoveInactiveClients();

            // Clients that are fully initialised
            std::set<std::shared_ptr<IntegrationClient>> clients;
    };
} // namespace UKControllerPlugin::Integration
