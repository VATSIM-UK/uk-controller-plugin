#include "Not.h"

namespace UKControllerPlugin::IntentionCode {

    Not::Not(std::shared_ptr<Condition> condition) : condition(condition)
    {
    }

    auto Not::Passes(
        Euroscope::EuroScopeCFlightPlanInterface& flightplan, Euroscope::EuroScopeCRadarTargetInterface& radarTarget)
        -> bool
    {
        return !condition->Passes(flightplan, radarTarget);
    }
} // namespace UKControllerPlugin::IntentionCode
