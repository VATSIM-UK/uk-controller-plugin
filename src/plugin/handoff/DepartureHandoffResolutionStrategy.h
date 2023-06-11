#pragma once

namespace UKControllerPlugin::Euroscope {
    class EuroScopeCFlightPlanInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::Handoff {

    struct ResolvedHandoff;

    /**
     * An interface for the strategy used to resolve a departure handoff.
     */
    class DepartureHandoffResolutionStrategy
    {
        public:
        virtual ~DepartureHandoffResolutionStrategy() = default;

        /**
         * Should return a resolved handoff for the given flight plan. It should never return nullptr.
         */
        virtual auto Resolve(const Euroscope::EuroScopeCFlightPlanInterface& flightPlan) const
            -> std::shared_ptr<const ResolvedHandoff> = 0;
    };
} // namespace UKControllerPlugin::Handoff
