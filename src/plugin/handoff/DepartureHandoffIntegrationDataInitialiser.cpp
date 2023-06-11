#include "DepartureHandoffIntegrationDataInitialiser.h"
#include "DepartureHandoffResolver.h"
#include "HandoffFrequencyUpdatedMessage.h"
#include "ResolvedHandoff.h"
#include "controller/ControllerPosition.h"
#include "datablock/DatablockFunctions.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "integration/IntegrationClient.h"
#include "integration/IntegrationConnection.h"

namespace UKControllerPlugin::Handoff {

    DepartureHandoffIntegrationDataInitialiser::DepartureHandoffIntegrationDataInitialiser(
        std::shared_ptr<DepartureHandoffResolver> resolver, Euroscope::EuroscopePluginLoopbackInterface& plugin)
        : resolver(resolver), plugin(plugin)
    {
        assert(resolver && "Resolver not set in ctor");
    }

    auto DepartureHandoffIntegrationDataInitialiser::InitialisesFor() const -> Integration::MessageType
    {
        return (HandoffFrequencyUpdatedMessage("", "")).GetMessageType();
    }

    void DepartureHandoffIntegrationDataInitialiser::Initialise(const Integration::IntegrationClient& client)
    {
        plugin.ApplyFunctionToAllFlightplans([this, &client](
                                                 const std::shared_ptr<Euroscope::EuroScopeCFlightPlanInterface>& fp,
                                                 const std::shared_ptr<Euroscope::EuroScopeCRadarTargetInterface>& rt) {
            auto resolved = resolver->Resolve(*fp);
            if (resolved) {
                client.Connection()->Send(std::make_shared<HandoffFrequencyUpdatedMessage>(
                    resolved->callsign,
                    Datablock::FrequencyStringFromDouble(resolved->resolvedController->GetFrequency())));
            }
        });
    }
} // namespace UKControllerPlugin::Handoff
