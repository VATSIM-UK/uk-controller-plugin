#pragma once

namespace UKControllerPlugin::Euroscope {
    class EuroScopeCRadarTargetInterface;
    class EuroScopeCFlightPlanInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::IntentionCode {

    /**
     * An interface for conditions that determine whether or not an intention code
     * is applicable.
     */
    class Condition
    {
        public:
        virtual ~Condition() = default;
        /*
         * Should return true if the condition is applicable and hence the intention code should be used.
         */
        [[nodiscard]] virtual auto Passes(
            const Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            const Euroscope::EuroScopeCRadarTargetInterface& radarTarget) -> bool = 0;
    };
} // namespace UKControllerPlugin::IntentionCode
