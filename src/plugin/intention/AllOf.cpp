#include "AllOf.h"

namespace UKControllerPlugin::IntentionCode {

    AllOf::AllOf(std::list<std::shared_ptr<Condition>> conditions) : conditions(std::move(conditions))
    {
    }

    auto AllOf::Passes(
        const Euroscope::EuroScopeCFlightPlanInterface& flightplan,
        const Euroscope::EuroScopeCRadarTargetInterface& radarTarget) -> bool
    {
        for (const auto& condition : conditions) {
            if (!condition->Passes(flightplan, radarTarget)) {
                return false;
            }
        }

        return true;
    }

    auto AllOf::Subconditions() const -> const std::list<std::shared_ptr<Condition>>&
    {
        return conditions;
    }
} // namespace UKControllerPlugin::IntentionCode
