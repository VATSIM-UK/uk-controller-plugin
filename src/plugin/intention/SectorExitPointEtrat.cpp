#include "SectorExitPointEtrat.h"
#include "euroscope/EuroscopeExtractedRouteInterface.h"

using UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface;

namespace UKControllerPlugin::IntentionCode {

    std::string SectorExitPointEtrat::GetIntentionCode(
        EuroscopeExtractedRouteInterface& route, int foundPointIndex, int cruiseLevel) const
    {
        const int numberOfPointsInRoute = route.GetPointsNumber();

        if (numberOfPointsInRoute > 0 &&
            destinations.find(route.GetPointName(numberOfPointsInRoute - 1)) != destinations.cend()) {
            return "E3";
        }

        // Special low level rule for ETRAT
        if (cruiseLevel <= 29000) {
            return "E2";
        }

        return SectorExitPoint::GetIntentionCode(route, foundPointIndex, cruiseLevel);
    }
} // namespace UKControllerPlugin::IntentionCode
