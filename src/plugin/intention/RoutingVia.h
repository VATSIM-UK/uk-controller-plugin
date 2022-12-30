#pragma once
#include "Condition.h"

namespace UKControllerPlugin::IntentionCode {

    /**
     * A condition that only passes if the aircraft is routing via a given point.
     */
    class RoutingVia : public Condition
    {
        public:
        RoutingVia(std::string via);
        [[nodiscard]] auto Passes(
            Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            Euroscope::EuroScopeCRadarTargetInterface& radarTarget) -> bool override;

        private:
        // The airfield pattern to match
        std::string via;
    };
} // namespace UKControllerPlugin::IntentionCode
