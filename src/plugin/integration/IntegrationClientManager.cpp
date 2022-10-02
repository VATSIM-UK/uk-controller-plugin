#include "integration/IntegrationClientManager.h"
#include "IntegrationConnection.h"
#include "integration/IntegrationClient.h"
#include "integration/MessageInterface.h"

namespace UKControllerPlugin::Integration {
    IntegrationClientManager::~IntegrationClientManager()
    {
        LogInfo("Shutting down integration clients");
    }

    void IntegrationClientManager::AddClient(std::shared_ptr<IntegrationClient> client)
    {
        if (!this->clients.insert(client).second) {
            LogWarning("Duplicate integration client added");
        }
    }

    void IntegrationClientManager::TimedEventTrigger()
    {
        this->RemoveInactiveClients();
    }

    std::shared_ptr<IntegrationClient> IntegrationClientManager::GetById(int id) const
    {
        auto client = std::find_if(
            this->clients.cbegin(),
            this->clients.cend(),
            [id](const std::shared_ptr<IntegrationClient>& client) -> bool { return client->Id() == id; });

        return client == this->clients.cend() ? nullptr : *client;
    }

    size_t IntegrationClientManager::CountClients() const
    {
        return this->clients.size();
    }

    void IntegrationClientManager::RemoveInactiveClients()
    {
        for (auto client = this->clients.begin(); client != this->clients.end();) {
            if (!(*client)->Connection()->Active()) {
                this->clients.erase(client++);
                continue;
            }

            ++client;
        }
    }
} // namespace UKControllerPlugin::Integration
