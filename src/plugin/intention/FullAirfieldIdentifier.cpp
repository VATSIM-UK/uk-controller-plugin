#include "FullAirfieldIdentifier.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"

namespace UKControllerPlugin::IntentionCode {
    auto FullAirfieldIdentifier::GenerateCode(const Euroscope::EuroScopeCFlightPlanInterface& flightplan) const
        -> std::string
    {
        if (flightplan.GetDestination().size() != 4) {
            return "--";
        }

        return flightplan.GetDestination();
    }
} // namespace UKControllerPlugin::IntentionCode
