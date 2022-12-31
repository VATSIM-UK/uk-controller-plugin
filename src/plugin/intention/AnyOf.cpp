#include "AnyOf.h"

namespace UKControllerPlugin::IntentionCode {

    AnyOf::AnyOf(std::list<std::shared_ptr<Condition>> conditions) : conditions(std::move(conditions))
    {
    }

    auto AnyOf::Passes(
        Euroscope::EuroScopeCFlightPlanInterface& flightplan, Euroscope::EuroScopeCRadarTargetInterface& radarTarget)
        -> bool
    {
        for (const auto& condition : conditions) {
            if (condition->Passes(flightplan, radarTarget)) {
                return true;
            }
        }

        return false;
    }

    auto AnyOf::Subconditions() const -> const std::list<std::shared_ptr<Condition>>&
    {
        return conditions;
    }
} // namespace UKControllerPlugin::IntentionCode
