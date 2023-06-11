#include "AircraftIntentionCode.h"
#include "AircraftIntentionCodeGenerator.h"
#include "FirExitPoint.h"
#include "FirExitPointCollection.h"
#include "IntentionCodeIntegrationDataInitialiser.h"
#include "IntentionCodeMetadata.h"
#include "IntentionCodeModel.h"
#include "IntentionCodeUpdatedMessage.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "integration/IntegrationClient.h"
#include "integration/IntegrationConnection.h"

namespace UKControllerPlugin::IntentionCode {

    IntentionCodeIntegrationDataInitialiser::IntentionCodeIntegrationDataInitialiser(
        std::shared_ptr<AircraftIntentionCodeGenerator> codeGenerator,
        std::shared_ptr<const FirExitPointCollection> exitPoints,
        Euroscope::EuroscopePluginLoopbackInterface& pluginLoopbackInterface)
        : codeGenerator(std::move(codeGenerator)), exitPoints(std::move(exitPoints)),
          pluginLoopbackInterface(pluginLoopbackInterface)
    {
        assert(this->codeGenerator && "Code generator not set in IntentionCodeIntegrationDataInitialiser");
        assert(this->exitPoints && "Exit points not set in IntentionCodeIntegrationDataInitialiser");
    }

    auto IntentionCodeIntegrationDataInitialiser::InitialisesFor() const -> Integration::MessageType
    {
        return (IntentionCodeUpdatedMessage("", "", "")).GetMessageType();
    }

    void IntentionCodeIntegrationDataInitialiser::Initialise(const Integration::IntegrationClient& client)
    {
        pluginLoopbackInterface.ApplyFunctionToAllFlightplans(
            [this, &client](
                std::shared_ptr<Euroscope::EuroScopeCFlightPlanInterface> fp,
                std::shared_ptr<Euroscope::EuroScopeCRadarTargetInterface> rt) {
                const auto intentionCode = codeGenerator->Generate(*fp, *rt);
                if (!intentionCode) {
                    return;
                }

                const auto exitPoint =
                    intentionCode->matchedIntentionCode
                        ? exitPoints->PointById(intentionCode->matchedIntentionCode->Metadata().exitPoint)
                        : nullptr;

                client.Connection()->Send(std::make_shared<IntentionCodeUpdatedMessage>(
                    intentionCode->callsign, exitPoint ? exitPoint->Identifier() : "", intentionCode->intentionCode));
            });
    }
} // namespace UKControllerPlugin::IntentionCode
