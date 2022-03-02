#include "LeadWakeCallsignProvider.h"
#include "WakeCalculatorOptions.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "ownership/AirfieldServiceProviderCollection.h"

namespace UKControllerPlugin::Wake {
    LeadWakeCallsignProvider::LeadWakeCallsignProvider(
        const Ownership::AirfieldServiceProviderCollection& serviceProvisions,
        Euroscope::EuroscopePluginLoopbackInterface& plugin,
        std::shared_ptr<WakeCalculatorOptions> options)
        : serviceProvisions(serviceProvisions), plugin(plugin), options(options)
    {
    }

    auto LeadWakeCallsignProvider::GetCallsigns() -> std::set<std::string>
    {
        std::set<std::string> callsigns;
        auto airfields = serviceProvisions.GetAirfieldsWhereUserProvidingServices(
            Ownership::ServiceType::Delivery | Ownership::ServiceType::Ground | Ownership::ServiceType::Tower);

        plugin.ApplyFunctionToAllFlightplans(
            [&callsigns, &airfields, this](
                const std::shared_ptr<Euroscope::EuroScopeCFlightPlanInterface>& flightplan,
                const std::shared_ptr<Euroscope::EuroScopeCRadarTargetInterface>& radarTarget) {
                if (std::find(airfields.begin(), airfields.end(), RelevantAirfieldForFlightplan(*flightplan)) ==
                    airfields.cend()) {
                    return;
                }

                callsigns.insert(flightplan->GetCallsign());
            });
        return callsigns;
    }

    void LeadWakeCallsignProvider::CallsignSelected(const std::string& callsign)
    {
        options->LeadAircraft(callsign);
        options->FollowingAircraft("");
    }

    auto LeadWakeCallsignProvider::RelevantAirfieldForFlightplan(
        const Euroscope::EuroScopeCFlightPlanInterface& flightplan) const -> std::string
    {
        return this->options->Departures() ? flightplan.GetOrigin() : flightplan.GetDestination();
    }
} // namespace UKControllerPlugin::Wake
