#pragma once

namespace UKControllerPlugin::Integration {

    class IntegrationDataInitialiser;
    class IntegrationClient;

    /**
     * Collects integration initialisers
     */
    class IntegrationDataInitialisers
    {
        public:
        void Add(const std::shared_ptr<IntegrationDataInitialiser>& initialiser);
        [[nodiscard]] auto Count() const -> size_t;
        void InitialiseClient(const IntegrationClient& client) const;

        private:
        [[nodiscard]] auto HasInitialiser(std::shared_ptr<IntegrationDataInitialiser> initialiser) const -> bool;

        // The initialisers
        std::list<std::shared_ptr<IntegrationDataInitialiser>> initialisers;
    };
} // namespace UKControllerPlugin::Integration
