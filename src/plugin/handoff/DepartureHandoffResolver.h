#pragma once

namespace UKControllerPlugin::Euroscope {
    class EuroScopeCFlightPlanInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::Handoff {
    struct ResolvedHandoff;

    class DepartureHandoffResolver
    {
        public:
        virtual ~DepartureHandoffResolver() = default;

        /**
         * Should resolve the handoff for the given flight plan. It should never return null.
         */
        [[nodiscard]] virtual auto Resolve(const Euroscope::EuroScopeCFlightPlanInterface& flightplan)
            -> std::shared_ptr<const ResolvedHandoff> = 0;

        /**
         * Should invalidate any caches or data persisted as part of the resolution process.
         */
        virtual void Invalidate(const Euroscope::EuroScopeCFlightPlanInterface& flightplan) = 0;
    };
} // namespace UKControllerPlugin::Handoff
