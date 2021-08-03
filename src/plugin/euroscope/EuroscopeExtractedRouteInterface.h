#pragma once
#include "euroscope/EuroScopePlugIn.h"

namespace UKControllerPlugin {
    namespace Euroscope {
        /*
            A wrapper around the Euroscope class, so that we can mock it and thus
            unit test.
        */
        class EuroscopeExtractedRouteInterface
        {
            public:
                explicit EuroscopeExtractedRouteInterface(EuroScopePlugIn::CFlightPlanExtractedRoute route);
                virtual ~EuroscopeExtractedRouteInterface(void) {}
                virtual int GetPointDistanceInMinutes(int index);
                virtual int GetPointsAssignedIndex(void);
                virtual int GetPointsCalculatedIndex(void);
                virtual int GetPointsNumber(void);
                virtual const char * GetPointName(int index);
                virtual EuroScopePlugIn::CPosition GetPointPosition(int index);

                // The value returned by GetPointDistanceInMinutes if the point is passed.
                const int pointPassed = -1;

                // The value returned by GetPointsAssignedIndex if no direct given.
                const int noDirect = -1;

            private:
                // The actual route from EuroScope
                EuroScopePlugIn::CFlightPlanExtractedRoute route;
            };
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
