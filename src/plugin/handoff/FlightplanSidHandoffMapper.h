#pragma once

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroScopeCFlightPlanInterface;
    } // namespace Euroscope
    namespace Sid {
        class SidMapperInterface;
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
        FlightplanSidHandoffMapper(const HandoffCollection& handoffs, const Sid::SidMapperInterface& sidMapper);

        [[nodiscard]] auto MapForFlightplan(const Euroscope::EuroScopeCFlightPlanInterface& flightplan) const
            -> std::shared_ptr<HandoffOrder>;

        private:
        // All the handoffs
        const HandoffCollection& handoffs;

        // All the sids
        const Sid::SidMapperInterface& sidMapper;
    };
} // namespace UKControllerPlugin::Handoff
