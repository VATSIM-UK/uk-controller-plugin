#include "AirfieldIdentifier.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"

namespace UKControllerPlugin::IntentionCode {
    auto AirfieldIdentifier::GenerateCode(const Euroscope::EuroScopeCFlightPlanInterface& flightplan) const
        -> std::string
    {
        if (flightplan.GetDestination().size() != 4) {
            return "--";
        }

        return flightplan.GetDestination().substr(2, 2);
    }
} // namespace UKControllerPlugin::IntentionCode
