#pragma once

namespace UKControllerPlugin {
    namespace Airfield {
        class AirfieldCollection;
    } // namespace Airfield
    namespace Euroscope {
        class EuroScopeCFlightPlanInterface;
    } // namespace Euroscope
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Handoff {
    class HandoffCollection;
    struct HandoffOrder;

    /**
     * Given a flightplan, finds the handoff for its departure airfield
     */
    class FlightplanAirfieldHandoffMapper
    {
        public:
        FlightplanAirfieldHandoffMapper(
            const HandoffCollection& handoffs, const Airfield::AirfieldCollection& airfields);
        [[nodiscard]] auto MapForFlightplan(const Euroscope::EuroScopeCFlightPlanInterface& flightplan) const
            -> std::shared_ptr<HandoffOrder>;

        private:
        // All the handoffs
        const HandoffCollection& handoffs;

        // The airfields
        const Airfield::AirfieldCollection& airfields;
    };
} // namespace UKControllerPlugin::Handoff
