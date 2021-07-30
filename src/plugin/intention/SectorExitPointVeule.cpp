#include "pch/pch.h"
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
                    return this->GetCodeForRoute("1", cruiseLevel);
                }

                // Via PEKIM
                if (strcmp(route.GetPointName(foundPointIndex), "PEKIM") == 0) {
                    return this->GetCodeForRoute("2", cruiseLevel);
                }

                // Via ODEBU - different code if crusing at FL270 or below.
                if (strcmp(route.GetPointName(foundPointIndex), "ODEBU") == 0) {
                    return this->GetCodeForRoute("3", cruiseLevel);
                }

                // Via KOTAP
                if (strcmp(route.GetPointName(foundPointIndex), "KOTAP") == 0) {
                    return this->GetCodeForRoute("5", cruiseLevel);
                }

                // Via DEKOD
                if (strcmp(route.GetPointName(foundPointIndex), "DEKOD") == 0) {
                    return this->GetCodeForRoute("6", cruiseLevel);
                }

                foundPointIndex++;
            }

            return SectorExitPoint::GetIntentionCode(route, foundPointIndex, cruiseLevel);
        }

        std::string SectorExitPointVeule::GetCodeForRoute(std::string routeNumber, int cruiseLevel) const
        {
            return cruiseLevel > 29000 ? "V" + routeNumber : "X" + routeNumber;
        }
    }  // namespace IntentionCode
}  // namespace UKControllerPlugin
