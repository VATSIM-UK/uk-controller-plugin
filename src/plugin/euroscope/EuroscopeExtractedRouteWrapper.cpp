#include "euroscope/EuroscopeExtractedRouteWrapper.h"

namespace UKControllerPlugin::Euroscope {

    EuroscopeExtractedRouteWrapper::EuroscopeExtractedRouteWrapper(EuroScopePlugIn::CFlightPlanExtractedRoute route)
    {
        this->route = route;
    }

    auto EuroscopeExtractedRouteWrapper::GetPointDistanceInMinutes(int index) -> int
    {
        return this->route.GetPointDistanceInMinutes(index);
    }

    auto EuroscopeExtractedRouteWrapper::GetPointsAssignedIndex(void) -> int
    {
        return this->route.GetPointsAssignedIndex();
    }

    auto EuroscopeExtractedRouteWrapper::GetPointsCalculatedIndex(void) -> int
    {
        return this->route.GetPointsCalculatedIndex();
    }

    auto EuroscopeExtractedRouteWrapper::GetPointsNumber(void) -> int
    {
        return this->route.GetPointsNumber();
    }

    auto EuroscopeExtractedRouteWrapper::GetPointName(int index) -> const char*
    {
        return this->route.GetPointName(index);
    }

    auto EuroscopeExtractedRouteWrapper::GetPointPosition(int index) -> EuroScopePlugIn::CPosition
    {
        return this->route.GetPointPosition(index);
    }
} // namespace UKControllerPlugin::Euroscope
