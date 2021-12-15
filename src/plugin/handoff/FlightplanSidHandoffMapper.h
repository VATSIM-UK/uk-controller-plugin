#pragma once

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroScopeCFlightPlanInterface;
    } // namespace Euroscope
    namespace Sid {
        class SidCollection;
    } // namespace Sid
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Handoff {
    class HandoffCollection;
    struct HandoffOrder;

    /**
     * Given a flightplan, map it to its SID
     * and in turn, to the appropriate handoff order.
     */
    class FlightplanSidHandoffMapper
    {
        public:
        FlightplanSidHandoffMapper(std::shared_ptr<const HandoffCollection> handoffs, const Sid::SidCollection& sids);

        [[nodiscard]] auto MapForFlightplan(const Euroscope::EuroScopeCFlightPlanInterface& flightplan) const
            -> std::shared_ptr<HandoffOrder>;

        private:
        // All the handoffs
        const std::shared_ptr<const HandoffCollection> handoffs;

        // All the sids
        const Sid::SidCollection& sids;
    };
} // namespace UKControllerPlugin::Handoff
