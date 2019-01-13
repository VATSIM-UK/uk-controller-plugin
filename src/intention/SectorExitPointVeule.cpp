#include "pch/stdafx.h"
#include "intention/SectorExitPointVeule.h"
#include "euroscope/EuroscopeExtractedRouteInterface.h"
#include "intention/IntentionCodeGenerator.h"

using UKControllerPlugin::IntentionCode::IntentionCodeGenerator;
using UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface;

namespace UKControllerPlugin {
    namespace IntentionCode {

        /*
            Returns the intention code dependent on onward routing and flight level.
        */
        std::string SectorExitPointVeule::GetIntentionCode(
            EuroscopeExtractedRouteInterface & route,
            int foundPointIndex,
            int cruiseLevel
        ) const {

            while (foundPointIndex < route.GetPointsNumber()) {

                // Via TEPRI
                if (strcmp(route.GetPointName(foundPointIndex), "TEPRI") == 0) {
                    return "V1";
                }

                // Via PEKIM
                if (strcmp(route.GetPointName(foundPointIndex), "PEKIM") == 0) {
                    return "V2";
                }

                // Via ODEBU - different code if crusing at FL270 or below.
                if (strcmp(route.GetPointName(foundPointIndex), "ODEBU") == 0) {
                    if (cruiseLevel <= 27000) {
                        return "V4";
                    }

                    return "V3";
                }

                // Via KOTAP
                if (strcmp(route.GetPointName(foundPointIndex), "KOTAP") == 0) {
                    return "V5";
                }

                // Via DEKOD
                if (strcmp(route.GetPointName(foundPointIndex), "DEKOD") == 0) {
                    return "V6";
                }

                foundPointIndex++;
            }

            return SectorExitPoint::GetIntentionCode(route, foundPointIndex, cruiseLevel);
        }
    }  // namespace IntentionCode
}  // namespace UKControllerPlugin
