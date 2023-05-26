#pragma once

namespace UKControllerPlugin::Integration {

    class IntegrationClient;
    struct MessageType;

    /**
     * Given an integration client, initialise it with whatever data it needs.
     */
    class IntegrationDataInitialiser
    {
        public:
        virtual ~IntegrationDataInitialiser() = default;
        [[nodiscard]] virtual auto InitialisesFor() const -> MessageType = 0;
        virtual void Initialise(const IntegrationClient& client) = 0;
    };
} // namespace UKControllerPlugin::Integration
