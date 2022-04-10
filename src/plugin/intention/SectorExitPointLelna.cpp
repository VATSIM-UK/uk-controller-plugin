#include "SectorExitPointLelna.h"
#include "euroscope/EuroscopeExtractedRouteInterface.h"

using UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface;

namespace UKControllerPlugin::IntentionCode {

    /*
       Look at the rest of the route to see where the aircraft goes in order to determine
       the intention code.
    */
    std::string SectorExitPointLelna::GetIntentionCode(
        EuroscopeExtractedRouteInterface& route, int foundPointIndex, int cruiseLevel) const
    {
        const int numberOfPointsInRoute = route.GetPointsNumber();

        // The arrival airport is always the last point on the route
        if (numberOfPointsInRoute >= 2) {
            std::string arrivalAirport = route.GetPointName(numberOfPointsInRoute - 1);
            if (arrivalAirport.substr(0, 3) == "LFR" && arrivalAirport != "LFRC") {
                return "H5";
            }

            if (arrivalAirport == "LFRC") {
                return "JC";
            }
        }

        while (foundPointIndex < numberOfPointsInRoute) {
            if (strcmp(route.GetPointName(foundPointIndex), "DOMOK") == 0) {
                return "H7";
            }

            foundPointIndex++;
        }

        return SectorExitPoint::GetIntentionCode(route, foundPointIndex, cruiseLevel);
    }
} // namespace UKControllerPlugin::IntentionCode
