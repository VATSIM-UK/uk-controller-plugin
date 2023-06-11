#pragma once
#include "integration/IntegrationDataInitialiser.h"

namespace UKControllerPlugin::Euroscope {
    class EuroscopePluginLoopbackInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::Handoff {

    class DepartureHandoffResolver;

    class DepartureHandoffIntegrationDataInitialiser : public Integration::IntegrationDataInitialiser
    {
        public:
        DepartureHandoffIntegrationDataInitialiser(
            std::shared_ptr<DepartureHandoffResolver> resolver, Euroscope::EuroscopePluginLoopbackInterface& plugin);
        auto InitialisesFor() const -> Integration::MessageType override;
        void Initialise(const Integration::IntegrationClient& client) override;

        private:
        // The resolver
        const std::shared_ptr<DepartureHandoffResolver> resolver;

        // The plugin
        Euroscope::EuroscopePluginLoopbackInterface& plugin;
    };
} // namespace UKControllerPlugin::Handoff
