#pragma once
#include "Condition.h"

namespace UKControllerPlugin::IntentionCode {

    /**
     * A condition that only passes if the subcondition fails
     */
    class Not : public Condition
    {
        public:
        Not(std::shared_ptr<Condition> condition);
        [[nodiscard]] auto Subcondition() const -> const Condition&;
        [[nodiscard]] auto Passes(
            const Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            const Euroscope::EuroScopeCRadarTargetInterface& radarTarget) -> bool override;

        private:
        // The conditions that must be true
        std::shared_ptr<Condition> condition;
    };
} // namespace UKControllerPlugin::IntentionCode
