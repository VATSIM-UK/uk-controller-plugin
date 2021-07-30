#include "pch/pch.h"
#include "intention/SectorExitPointKonan.h"
#include "euroscope/EuroscopeExtractedRouteInterface.h"

using UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface;

namespace UKControllerPlugin {
    namespace IntentionCode {

        std::string SectorExitPointKonan::GetIntentionCode(
            EuroscopeExtractedRouteInterface & route,
            int foundPointIndex,
            int cruiseLevel
        ) const
        {
            int numberOfPoints = route.GetPointsNumber();
            while (foundPointIndex < numberOfPoints) {

                // If going via KOK, the intention code is D1
                if (strcmp(route.GetPointName(foundPointIndex), "KOK") == 0) {
                    return "D1";
                }

                foundPointIndex++;
            }

            // If not via KOK, we fall back to the default code
            return SectorExitPoint::GetIntentionCode(route, foundPointIndex, cruiseLevel);
        }
    }  // namespace IntentionCode
}  // namespace UKControllerPlugin
