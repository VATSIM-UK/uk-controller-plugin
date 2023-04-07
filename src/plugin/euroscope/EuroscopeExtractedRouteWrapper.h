#pragma once
#include "euroscope/EuroscopeExtractedRouteInterface.h"

namespace UKControllerPlugin::Euroscope {
    /*
        A wrapper around the Euroscope class, so that we can mock it and thus
        unit test.
    */
    class EuroscopeExtractedRouteWrapper : public EuroscopeExtractedRouteInterface
    {
        public:
        EuroscopeExtractedRouteWrapper(EuroScopePlugIn::CFlightPlanExtractedRoute route);
        [[nodiscard]] auto GetPointDistanceInMinutes(int index) -> int override;
        [[nodiscard]] auto GetPointsAssignedIndex() -> int override;
        [[nodiscard]] auto GetPointsCalculatedIndex() -> int override;
        [[nodiscard]] auto GetPointsNumber() -> int override;
        [[nodiscard]] auto GetPointName(int index) -> const char* override;
        [[nodiscard]] auto GetPointPosition(int index) -> EuroScopePlugIn::CPosition override;

        private:
        // The actual route from EuroScope
        EuroScopePlugIn::CFlightPlanExtractedRoute route;
    };
} // namespace UKControllerPlugin::Euroscope
