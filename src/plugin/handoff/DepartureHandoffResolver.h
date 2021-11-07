#pragma once

namespace UKControllerPlugin {
    namespace Controller {
        class ActiveCallsignCollection;
    } // namespace Controller
    namespace Euroscope {
        class EuroScopeCFlightPlanInterface;
    } // namespace Euroscope
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Handoff {
    class FlightplanSidHandoffMapper;
    class ResolvedHandoff;

    /**
     * Given a flightplan, resolves the handoff frequency
     * that should be used for the after departure TAG item.
     */
    class DepartureHandoffResolver
    {
        public:
        DepartureHandoffResolver(
            std::shared_ptr<FlightplanSidHandoffMapper> mapper,
            const Controller::ActiveCallsignCollection& activeCallsigns);

        [[nodiscard]] auto Resolve(Euroscope::EuroScopeCFlightPlanInterface& flightplan) const
            -> std::shared_ptr<ResolvedHandoff>;

        private:
        // Maps flightplans to sids to handoffs
        const std::shared_ptr<FlightplanSidHandoffMapper> mapper;

        // All the active controllers
        const Controller::ActiveCallsignCollection& activeCallsigns;
    };
} // namespace UKControllerPlugin::Handoff
