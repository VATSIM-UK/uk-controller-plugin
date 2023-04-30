#pragma once
#include "departure/AircraftDepartedEvent.h"
#include "eventhandler/EventHandler.h"

namespace UKControllerPlugin {
    namespace Handoff {
        class HandoffCache;
    } // namespace Handoff
    namespace Ownership {
        class AirfieldServiceProviderCollection;
    } // namespace Ownership
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Departure {

    class UserShouldClearDepartureDataMonitor : public EventHandler::EventHandler<Departure::AircraftDepartedEvent>
    {
        public:
        UserShouldClearDepartureDataMonitor(
            std::shared_ptr<const Handoff::HandoffCache> handoffs,
            std::shared_ptr<Ownership::AirfieldServiceProviderCollection> ownership);
        void OnEvent(const Departure::AircraftDepartedEvent& event);

        private:
        [[nodiscard]] auto UserIsResponsibleForClearingData(const std::string& airfield) -> bool;

        // For getting handoffs
        std::shared_ptr<const Handoff::HandoffCache> handoffs;

        // For getting ownership
        std::shared_ptr<Ownership::AirfieldServiceProviderCollection> ownership;
    };
} // namespace UKControllerPlugin::Departure
