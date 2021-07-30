#pragma once
#include "timedevent/AbstractTimedEvent.h"

namespace UKControllerPlugin::Integration {
    class IntegrationClient;
    class IntegrationConnection;

    /*
     * Manages the lifetime of integration clients
     */
    class IntegrationClientManager : public TimedEvent::AbstractTimedEvent
    {
        public:
            IntegrationClientManager() = default;
            ~IntegrationClientManager() override;
            void AddClient(std::shared_ptr<IntegrationClient> client);
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
