#pragma once

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroScopeCFlightPlanInterface;
    } //
    namespace Sid {
        class SidCollection;
    } //
} // namespace UKControllerPlugin::Sid

namespace UKControllerPlugin::Handoff {
    class HandoffCollection;
    class HandoffOrder;

    /**
     * Given a flightplan, map it to its SID
     * and in turn, to the appropriate handoff order.
     */
    class FlightplanSidHandoffMapper
    {
        public:
        FlightplanSidHandoffMapper(std::shared_ptr<const HandoffCollection> handoffs,
                                   std::shared_ptr<const Sid::SidCollection>
            sids);
        
        [[nodiscard]] auto MapForFlightplan(Euroscope::EuroScopeCFlightPlanInterface& flightplan) const ->
            std::shared_ptr<HandoffOrder>;
        
        private:
        
        // All the handoffs
        const std::shared_ptr<const HandoffCollection> handoffs;
        
        // All the sids
        std::shared_ptr<const Sid::SidCollection>
            sids;
    };
} // namespace UKControllerPlugin::Handoff
