#pragma once
#include "integration/IntegrationDataInitialiser.h"

namespace UKControllerPlugin::Euroscope {
    class EuroscopePluginLoopbackInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::IntentionCode {

    class AircraftIntentionCodeGenerator;
    class FirExitPointCollection;

    class IntentionCodeIntegrationDataInitialiser : public Integration::IntegrationDataInitialiser
    {
        public:
        IntentionCodeIntegrationDataInitialiser(
            std::shared_ptr<AircraftIntentionCodeGenerator> codeGenerator,
            std::shared_ptr<const FirExitPointCollection> exitPoints,
            Euroscope::EuroscopePluginLoopbackInterface& pluginLoopbackInterface);
        auto InitialisesFor() const -> Integration::MessageType override;
        void Initialise(const Integration::IntegrationClient& client) override;

        private:
        // Code generator
        std::shared_ptr<AircraftIntentionCodeGenerator> codeGenerator;

        // The FIR exit points
        std::shared_ptr<const FirExitPointCollection> exitPoints;

        // The plugin
        Euroscope::EuroscopePluginLoopbackInterface& pluginLoopbackInterface;
    };
} // namespace UKControllerPlugin::IntentionCode
