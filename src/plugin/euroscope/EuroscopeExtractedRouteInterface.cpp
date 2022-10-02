#include "euroscope/EuroscopeExtractedRouteInterface.h"

namespace UKControllerPlugin {
    namespace Euroscope {

        EuroscopeExtractedRouteInterface::EuroscopeExtractedRouteInterface(
            EuroScopePlugIn::CFlightPlanExtractedRoute route)
        {
            this->route = route;
        }

        int EuroscopeExtractedRouteInterface::GetPointDistanceInMinutes(int index)
        {
            return this->route.GetPointDistanceInMinutes(index);
        }

        int EuroscopeExtractedRouteInterface::GetPointsAssignedIndex(void)
        {
            return this->route.GetPointsAssignedIndex();
        }

        int EuroscopeExtractedRouteInterface::GetPointsCalculatedIndex(void)
        {
            return this->route.GetPointsCalculatedIndex();
        }

        int EuroscopeExtractedRouteInterface::GetPointsNumber(void)
        {
            return this->route.GetPointsNumber();
        }

        const char* EuroscopeExtractedRouteInterface::GetPointName(int index)
        {
            return this->route.GetPointName(index);
        }

        EuroScopePlugIn::CPosition EuroscopeExtractedRouteInterface::GetPointPosition(int index)
        {
            return this->route.GetPointPosition(index);
        }
    } // namespace Euroscope
} // namespace UKControllerPlugin
