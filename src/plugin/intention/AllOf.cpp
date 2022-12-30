#include "AllOf.h"

namespace UKControllerPlugin::IntentionCode {

    AllOf::AllOf(std::list<std::shared_ptr<Condition>> conditions) : conditions(std::move(conditions))
    {
    }

    auto AllOf::Passes(
        Euroscope::EuroScopeCFlightPlanInterface& flightplan, Euroscope::EuroScopeCRadarTargetInterface& radarTarget)
        -> bool
    {
        for (const auto& condition : conditions) {
            if (!condition->Passes(flightplan, radarTarget)) {
                return false;
            }
        }

        return true;
    }
} // namespace UKControllerPlugin::IntentionCode
