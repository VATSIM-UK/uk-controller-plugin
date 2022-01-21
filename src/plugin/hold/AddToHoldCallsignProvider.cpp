#include "AddToHoldCallsignProvider.h"
#include "HoldManager.h"
#include "HoldingAircraft.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "navaids/Navaid.h"

namespace UKControllerPlugin::Hold {

    AddToHoldCallsignProvider::AddToHoldCallsignProvider(
        const Navaids::Navaid& navaid, HoldManager& holdManager, Euroscope::EuroscopePluginLoopbackInterface& plugin)
        : navaid(navaid), holdManager(holdManager), plugin(plugin)
    {
    }

    auto AddToHoldCallsignProvider::GetCallsigns() -> std::set<std::string>
    {
        std::set<std::string> callsigns;
        const auto holdingAircraft = this->holdManager.GetAircraftForHold(navaid.identifier);

        this->plugin.ApplyFunctionToAllFlightplans(
            [&callsigns, &holdingAircraft](
                const std::shared_ptr<Euroscope::EuroScopeCFlightPlanInterface>& fp,
                const std::shared_ptr<Euroscope::EuroScopeCRadarTargetInterface>& rt) {
                if (!fp->IsTrackedByUser()) {
                    return;
                }

                const auto aircraftInHold = std::find_if(
                                                holdingAircraft.cbegin(),
                                                holdingAircraft.cend(),
                                                [&fp](const std::shared_ptr<HoldingAircraft>& aircraft) -> bool {
                                                    return aircraft->GetCallsign() == fp->GetCallsign();
                                                }) != holdingAircraft.cend();

                if (aircraftInHold) {
                    return;
                }

                callsigns.insert(fp->GetCallsign());
            });

        return callsigns;
    }

    void AddToHoldCallsignProvider::CallsignSelected(const std::string& callsign)
    {
        const auto fp = this->plugin.GetFlightplanForCallsign(callsign);
        if (!fp || !fp->IsTrackedByUser()) {
            LogWarning("Tried to add " + callsign + " to hold but not tracked by user");
            return;
        }

        this->holdManager.AssignAircraftToHold(fp->GetCallsign(), navaid.identifier, true);
    }
} // namespace UKControllerPlugin::Hold
