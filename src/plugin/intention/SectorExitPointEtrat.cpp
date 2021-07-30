#include "pch/pch.h"
#include "intention/SectorExitPointEtrat.h"
#include "euroscope/EuroscopeExtractedRouteInterface.h"

using UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface;

namespace UKControllerPlugin {
    namespace IntentionCode {

        std::string SectorExitPointEtrat::GetIntentionCode(
            EuroscopeExtractedRouteInterface & route,
            int foundPointIndex,
            int cruiseLevel
        ) const
        {
            // Special low level rule for ETRAT
            if (cruiseLevel <= 27000) {
                return "E2";
            }

            return SectorExitPoint::GetIntentionCode(route, foundPointIndex, cruiseLevel);
        }
    }  // namespace IntentionCode
}  // namespace UKControllerPlugin
