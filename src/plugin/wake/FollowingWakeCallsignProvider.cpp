#include "FollowingWakeCallsignProvider.h"
#include "WakeCalculatorOptions.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"

namespace UKControllerPlugin::Wake {

    FollowingWakeCallsignProvider::FollowingWakeCallsignProvider(
        Euroscope::EuroscopePluginLoopbackInterface& plugin, std::shared_ptr<WakeCalculatorOptions> options)
        : plugin(plugin), options(options)
    {
    }

    auto FollowingWakeCallsignProvider::GetCallsigns() -> std::set<std::string>
    {
        std::set<std::string> callsigns;
        const auto leadFlightplan = plugin.GetFlightplanForCallsign(this->options->LeadAircraft());
        if (!leadFlightplan) {
            return callsigns;
        }

        plugin.ApplyFunctionToAllFlightplans(
            [&callsigns, &leadFlightplan](
                const std::shared_ptr<Euroscope::EuroScopeCFlightPlanInterface>& flightplan,
                const std::shared_ptr<Euroscope::EuroScopeCRadarTargetInterface>& radarTarget) {
                if (leadFlightplan->GetCallsign() == flightplan->GetCallsign() ||
                    leadFlightplan->GetOrigin() != flightplan->GetOrigin()) {
                    return;
                }

                callsigns.insert(flightplan->GetCallsign());
            });
        return callsigns;
    }

    void FollowingWakeCallsignProvider::CallsignSelected(const std::string& callsign)
    {
        this->options->FollowingAircraft(callsign);
    }
} // namespace UKControllerPlugin::Wake
