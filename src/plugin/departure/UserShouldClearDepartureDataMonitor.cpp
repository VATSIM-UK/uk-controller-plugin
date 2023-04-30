#include "UserShouldClearDepartureDataEvent.h"
#include "UserShouldClearDepartureDataMonitor.h"
#include "controller/ControllerPosition.h"
#include "eventhandler/EventBus.h"
#include "handoff/HandoffCache.h"
#include "handoff/ResolvedHandoff.h"
#include "log/LoggerFunctions.h"
#include "ownership/AirfieldServiceProviderCollection.h"
#include "ownership/ServiceType.h"

using UKControllerPluginUtils::EventHandler::EventBus;

namespace UKControllerPlugin::Departure {

    UserShouldClearDepartureDataMonitor::UserShouldClearDepartureDataMonitor(
        std::shared_ptr<const Handoff::HandoffCache> handoffs,
        std::shared_ptr<Ownership::AirfieldServiceProviderCollection> ownership)
        : handoffs(std::move(handoffs)), ownership(std::move(ownership))
    {
        assert(this->handoffs && "Handoffs not set in ClearInitialAltitudeOnDeparture");
        assert(this->ownership && "Ownership not set in ClearInitialAltitudeOnDeparture");
    }

    void UserShouldClearDepartureDataMonitor::OnEvent(const Departure::AircraftDepartedEvent& event)
    {
        // Check for a handoff, dont do it if there's a handoff currently present.
        const auto handoff = handoffs->Get(event.callsign);
        if (!handoff) {
            LogDebug("Not firing UserShouldClearDepartureDataEvent for " + event.callsign + ", no handoff found");
            return;
        }

        // If the resolved controller id is > 0, then theres a handoff controller... -1 is UNICOM in
        // DepartureHandoffResolver
        if (handoff->resolvedController->GetId() > 0) {
            LogDebug(
                "Not firing UserShouldClearDepartureDataEvent for " + event.callsign + ", handoff controller online");
            return;
        }

        // If we're providing approach control, then we are the next controller due to top-down rules
        if (ownership->ApproachControlProvidedByUser(event.airfield)) {
            return;
        }

        if (!UserIsResponsibleForClearingData(event.airfield)) {
            return;
        }

        LogDebug("Firing UserShouldClearDepartureDataEvent for " + event.callsign);
        EventBus::Bus().OnEvent<UserShouldClearDepartureDataEvent>({event.callsign});
    }

    /**
     * A user is only responsible for clearing data if they're the closest thing to tower.
     */
    auto UserShouldClearDepartureDataMonitor::UserIsResponsibleForClearingData(const std::string& airfield) -> bool
    {
        return ownership->TowerControlProvidedByUser(airfield) ||
               (!ownership->ServiceProvidedAtAirfield(airfield, Ownership::ServiceType::Tower) &&
                ownership->GroundControlProvidedByUser(airfield)) ||
               (!ownership->ServiceProvidedAtAirfield(airfield, Ownership::ServiceType::Tower) &&
                !ownership->ServiceProvidedAtAirfield(airfield, Ownership::ServiceType::Ground) &&
                ownership->DeliveryControlProvidedByUser(airfield));
    }
} // namespace UKControllerPlugin::Departure
