#pragma once
#include "Condition.h"

namespace UKControllerPlugin::IntentionCode {

    /**
     * A condition that only passes if at least one subcondtion is true
     */
    class AnyOf : public Condition
    {
        public:
        AnyOf(std::list<std::shared_ptr<Condition>> conditions);
        [[nodiscard]] auto Passes(
            Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            Euroscope::EuroScopeCRadarTargetInterface& radarTarget) -> bool override;

        private:
        // The conditions that must be true
        std::list<std::shared_ptr<Condition>> conditions;
    };
} // namespace UKControllerPlugin::IntentionCode
