#include "intention/SectorExitPointEtrat.h"
#include "euroscope/EuroscopeExtractedRouteInterface.h"

using UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface;

namespace UKControllerPlugin::IntentionCode {

    std::string SectorExitPointEtrat::GetIntentionCode(
        EuroscopeExtractedRouteInterface& route, int foundPointIndex, int cruiseLevel) const
    {
        if (destinations.find(route.GetPointName(route.GetPointsNumber() - 1)) != destinations.cend()) {
            return "E3";
        }

        // Special low level rule for ETRAT
        if (cruiseLevel <= 27000) {
            return "E2";
        }

        return SectorExitPoint::GetIntentionCode(route, foundPointIndex, cruiseLevel);
    }
} // namespace UKControllerPlugin::IntentionCode
