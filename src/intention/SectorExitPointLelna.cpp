#include "pch/stdafx.h"
#include "intention/SectorExitPointLelna.h"
#include "euroscope/EuroscopeExtractedRouteInterface.h"
#include "intention/IntentionCodeGenerator.h"

using UKControllerPlugin::IntentionCode::IntentionCodeGenerator;
using UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface;

namespace UKControllerPlugin {
    namespace IntentionCode {

        /*
           Loop through the rest of the route to see where the aircraft goes in order to determine
           the intention code.
        */
        std::string SectorExitPointLelna::GetIntentionCode(
            EuroscopeExtractedRouteInterface & route,
            int foundPointIndex,
            int cruiseLevel
        ) const {
            while (foundPointIndex < route.GetPointsNumber()) {
                if (strcmp(route.GetPointName(foundPointIndex), "DIN") == 0) {
                    return "H2";
                }

                // Not ARE, but the airway continues to there.
                if (strcmp(route.GetPointName(foundPointIndex), "DOMOK") == 0 ||
                        strcmp(route.GetPointName(foundPointIndex), "ARE") == 0
                ) {
                    return "H3";
                }

                foundPointIndex++;
            }

            return SectorExitPoint::GetIntentionCode(route, foundPointIndex, cruiseLevel);
        }
    }  // namespace IntentionCode
}  // namespace UKControllerPlugin
