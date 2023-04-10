#include "UserShouldClearDepartureDataEvent.h"
#include "UserShouldClearDepartureDataMonitor.h"
#include "eventhandler/EventBus.h"
#include "handoff/HandoffCache.h"
#include "ownership/AirfieldServiceProviderCollection.h"
#include "ownership/ServiceType.h"

using UKControllerPlugin::EventHandler::EventBus;

namespace UKControllerPlugin::Departure {

    UserShouldClearDepartureDataMonitor::UserShouldClearDepartureDataMonitor(
        std::shared_ptr<Handoff::HandoffCache> handoffs,
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
        if (handoff) {
            return;
        }

        // If we're providing approach control, then we are the next controller due to top-down rules
        if (ownership->ApproachControlProvidedByUser(event.airfield)) {
            return;
        }

        if (!UserIsResponsibleForClearingData(event.airfield)) {
            return;
        }

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
