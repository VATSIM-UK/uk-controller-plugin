#include "ResetSquawkOnFailedDelete.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "flightplan/StoredFlightplanCollection.h"

namespace UKControllerPlugin::Squawk {

    ResetSquawkOnFailedDelete::ResetSquawkOnFailedDelete(
        Euroscope::EuroscopePluginLoopbackInterface& plugin, Flightplan::StoredFlightplanCollection& storedFlightplans)
        : plugin(plugin), storedFlightplans(storedFlightplans)
    {
    }

    void ResetSquawkOnFailedDelete::OnEvent(const SquawkAssignmentDeleteForConspicuityFailedEvent& event)
    {
        auto flightplan = this->plugin.GetFlightplanForCallsign(event.callsign);
        if (!flightplan) {
            return;
        }

        flightplan->SetSquawk(event.previousSquawk);

        // Update the stored flightplan
        if (!this->storedFlightplans.HasFlightplanForCallsign(event.callsign)) {
            return;
        }

        this->storedFlightplans.GetFlightplanForCallsign(event.callsign)
            .SetPreviouslyAssignedSquawk(event.previousSquawk);
    }
} // namespace UKControllerPlugin::Squawk
