#include "Not.h"

namespace UKControllerPlugin::IntentionCode {

    Not::Not(std::shared_ptr<Condition> condition) : condition(condition)
    {
    }

    auto Not::Passes(
        const Euroscope::EuroScopeCFlightPlanInterface& flightplan,
        const Euroscope::EuroScopeCRadarTargetInterface& radarTarget) -> bool
    {
        return !condition->Passes(flightplan, radarTarget);
    }

    auto Not::Subcondition() const -> const Condition&
    {
        return *condition;
    }
} // namespace UKControllerPlugin::IntentionCode
