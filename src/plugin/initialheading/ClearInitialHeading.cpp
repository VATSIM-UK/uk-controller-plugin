#include "ClearInitialHeading.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "log/LoggerFunctions.h"
#include "sid/SidMapperInterface.h"
#include "sid/StandardInstrumentDeparture.h"

namespace UKControllerPlugin::InitialHeading {

    ClearInitialHeading::ClearInitialHeading(
        const Euroscope::EuroscopePluginLoopbackInterface& plugin, const Sid::SidMapperInterface& sidMapper)
        : plugin(plugin), sidMapper(sidMapper)
    {
    }

    void ClearInitialHeading::OnEvent(const Departure::UserShouldClearDepartureDataEvent& event)
    {
        // Dont clear if flightplan is tracked by someone else
        const auto flightplan = plugin.GetFlightplanForCallsign(event.callsign);
        if (!flightplan) {
            LogDebug("Not removing cleared heading for " + event.callsign + " as disconnected");
            return;
        }

        if (flightplan->IsTracked() && !flightplan->IsTrackedByUser()) {
            LogDebug("Not removing cleared heading for " + event.callsign + " as tracked by someone else");
            return;
        }

        // Dont clear if the flightplan has a cleared level different to the sid level
        const auto sid = sidMapper.MapFlightplanToSid(*flightplan);
        if (!sid) {
            LogWarning("Sid not found when removing cleared heading for " + event.callsign);
            return;
        }

        if (flightplan->GetAssignedHeading() != sid->InitialHeading()) {
            LogDebug("Not clearing heading for " + event.callsign + " on departure, has been modified");
            return;
        }

        if (flightplan->GetAssignedHeading() == EUROSCOPE_FLIGHTPLAN_NO_HEADING) {
            LogDebug("Not clearing heading for " + event.callsign + " on departure, not set");
            return;
        }

        LogInfo("Removing cleared heading on departure for " + event.callsign);
        flightplan->SetHeading(EUROSCOPE_FLIGHTPLAN_NO_HEADING);
    }
} // namespace UKControllerPlugin::InitialHeading
