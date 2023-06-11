#pragma once
#include "departure/AircraftDepartedEvent.h"
#include "eventhandler/EventHandler.h"

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
    } // namespace Euroscope
    namespace Handoff {
        class DepartureHandoffResolver;
    } // namespace Handoff
    namespace Ownership {
        class AirfieldServiceProviderCollection;
    } // namespace Ownership
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Departure {

    class UserShouldClearDepartureDataMonitor
        : public UKControllerPluginUtils::EventHandler::EventHandler<Departure::AircraftDepartedEvent>
    {
        public:
        UserShouldClearDepartureDataMonitor(
            std::shared_ptr<Handoff::DepartureHandoffResolver> handoffResolver,
            std::shared_ptr<Ownership::AirfieldServiceProviderCollection> ownership,
            Euroscope::EuroscopePluginLoopbackInterface& plugin);
        void OnEvent(const Departure::AircraftDepartedEvent& event);

        private:
        [[nodiscard]] auto UserIsResponsibleForClearingData(const std::string& airfield) -> bool;

        // For getting handoffs
        std::shared_ptr<Handoff::DepartureHandoffResolver> handoffResolver;

        // For getting ownership
        std::shared_ptr<Ownership::AirfieldServiceProviderCollection> ownership;

        // For getting flightplans
        Euroscope::EuroscopePluginLoopbackInterface& plugin;
    };
} // namespace UKControllerPlugin::Departure
