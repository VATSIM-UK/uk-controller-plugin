#pragma once

namespace UKControllerPlugin::Integration {
    class IntegrationClient;
    class IntegrationConnection;
    /*
     * Manages the lifetime of and messages to
     * integration clients.
     */
    class IntegrationClientManager
    {
        public:
            IntegrationClientManager() = default;
            ~IntegrationClientManager();
            void AddClient(std::shared_ptr<IntegrationClient> client);

        private:
            // Clients that are fully initialised
            std::set<std::shared_ptr<IntegrationClient>> clients;
            std::set<std::shared_ptr<IntegrationConnection>> uninitialisedClients;
    };
} // namespace UKControllerPlugin::Integration
