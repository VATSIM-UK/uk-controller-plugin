#include "pch/stdafx.h"
#include "intention/SectorExitPointShanwick.h"
#include "euroscope/EuroscopeExtractedRouteInterface.h"
#include "intention/IntentionCodeGenerator.h"

using UKControllerPlugin::IntentionCode::IntentionCodeGenerator;
using UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface;


namespace UKControllerPlugin {
    namespace IntentionCode {
        std::string SectorExitPointShanwick::GetIntentionCode(
            EuroscopeExtractedRouteInterface & route,
            int foundPointIndex,
            int cruiseLevel
        ) const {
            // If we find Belfast in the route, apply the Belfast intention code.
            if (IsViaBelfast(route, foundPointIndex)) {
                return "U" + SectorExitPoint::GetIntentionCode(route, foundPointIndex, cruiseLevel).substr(1);
            }

            return SectorExitPoint::GetIntentionCode(route, foundPointIndex, cruiseLevel);
        }

        /*
            Checks the remainder of the route (starting from where an exit point was found) to see if that
            routes via Belfast VOR.
        */
        bool SectorExitPointShanwick::IsViaBelfast(EuroscopeExtractedRouteInterface & route, int foundPointIndex) const
        {
            while (foundPointIndex >= 0) {
                if (route.GetPointName(foundPointIndex) == "BEL") {
                    return true;
                }

                foundPointIndex--;
            }

            return false;
        }
    }  // namespace IntentionCode
}  // namespace UKControllerPlugin
