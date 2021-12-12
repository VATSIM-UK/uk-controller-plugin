#pragma once

namespace UKControllerPlugin {
    namespace Controller {
        class ActiveCallsignCollection;
        class ControllerPositionHierarchy;
    } // namespace Controller
    namespace Euroscope {
        class EuroScopeCFlightPlanInterface;
    } // namespace Euroscope
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Handoff {
    class FlightplanSidHandoffMapper;
    struct ResolvedHandoff;

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

        [[nodiscard]] auto Resolve(const Euroscope::EuroScopeCFlightPlanInterface& flightplan) const
            -> std::shared_ptr<ResolvedHandoff>;

        private:
        [[nodiscard]] static auto ResolveToUnicom(
            const Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            std::shared_ptr<Controller::ControllerPositionHierarchy> handoffOrder) -> std::shared_ptr<ResolvedHandoff>;

        // Maps flightplans to sids to handoffs
        const std::shared_ptr<FlightplanSidHandoffMapper> mapper;

        // All the active controllers
        const Controller::ActiveCallsignCollection& activeCallsigns;

        // The unicom frequency
        inline static const double UNICOM_FREQUENCY = 122.800;
    };
} // namespace UKControllerPlugin::Handoff
