#include "pch/stdafx.h"
#include "integration/IntegrationClientManager.h"
#include "integration/IntegrationClient.h"

namespace UKControllerPlugin::Integration {
    IntegrationClientManager::~IntegrationClientManager()
    {
        LogInfo("Shutting down integration clients");
        for (const auto client : this->clients) {
            client->Shutdown();
        }
    }

    void IntegrationClientManager::AddClient(std::shared_ptr<IntegrationClient> client)
    {
        if (!this->clients.insert(client).second) {
            LogWarning("Duplicate integration client added");
        }
    }
} // namespace UKControllerPlugin::Integration
