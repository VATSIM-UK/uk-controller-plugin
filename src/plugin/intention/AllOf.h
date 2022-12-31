#pragma once
#include "Condition.h"

namespace UKControllerPlugin::IntentionCode {

    /**
     * A condition that only passes if all the sub-conditions pass.
     */
    class AllOf : public Condition
    {
        public:
        AllOf(std::list<std::shared_ptr<Condition>> conditions);
        [[nodiscard]] auto Subconditions() const -> const std::list<std::shared_ptr<Condition>>&;
        [[nodiscard]] auto Passes(
            Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            Euroscope::EuroScopeCRadarTargetInterface& radarTarget) -> bool override;

        private:
        // The conditions that must be true
        std::list<std::shared_ptr<Condition>> conditions;
    };
} // namespace UKControllerPlugin::IntentionCode
