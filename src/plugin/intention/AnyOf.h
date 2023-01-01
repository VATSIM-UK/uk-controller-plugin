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
        [[nodiscard]] auto Subconditions() const -> const std::list<std::shared_ptr<Condition>>&;
        [[nodiscard]] auto Passes(
            const Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            const Euroscope::EuroScopeCRadarTargetInterface& radarTarget) -> bool override;

        private:
        // The conditions that must be true
        std::list<std::shared_ptr<Condition>> conditions;
    };
} // namespace UKControllerPlugin::IntentionCode
