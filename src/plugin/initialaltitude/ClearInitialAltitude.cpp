#include "ClearInitialAltitude.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "log/LoggerFunctions.h"
#include "sid/SidMapperInterface.h"
#include "sid/StandardInstrumentDeparture.h"

namespace UKControllerPlugin::InitialAltitude {

    ClearInitialAltitude::ClearInitialAltitude(
        const Euroscope::EuroscopePluginLoopbackInterface& plugin, const Sid::SidMapperInterface& sidMapper)
        : plugin(plugin), sidMapper(sidMapper)
    {
    }

    void ClearInitialAltitude::OnEvent(const Departure::UserShouldClearDepartureDataEvent& event)
    {
        // Dont clear if flightplan is tracked by someone else
        const auto flightplan = plugin.GetFlightplanForCallsign(event.callsign);
        if (!flightplan) {
            LogDebug("Not removing cleared level for " + event.callsign + " as disconnected");
            return;
        }

        if (flightplan->IsTracked() && !flightplan->IsTrackedByUser()) {
            LogDebug("Not removing cleared level for " + event.callsign + " as tracked by someone else");
            return;
        }

        // Dont clear if the flightplan has a cleared level different to the sid level
        const auto sid = sidMapper.MapFlightplanToSid(*flightplan);
        if (!sid) {
            LogWarning("Sid not found when removing cleared level for " + event.callsign);
            return;
        }

        if (flightplan->GetClearedAltitude() != sid->InitialAltitude()) {
            LogDebug("Not clearing level for " + event.callsign + " on departure, has been modified");
            return;
        }

        LogInfo("Removing cleared level on departure for " + event.callsign);
        flightplan->SetClearedAltitude(EUROSCOPE_FLIGHTPLAN_NO_CLEARED_LEVEL);
    }
} // namespace UKControllerPlugin::InitialAltitude
