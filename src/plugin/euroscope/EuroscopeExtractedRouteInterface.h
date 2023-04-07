#pragma once

namespace UKControllerPlugin::Euroscope {
    /*
        An interface around the Euroscope class, so that we can mock it and thus
        unit test.
    */
    class EuroscopeExtractedRouteInterface
    {
        public:
        virtual ~EuroscopeExtractedRouteInterface() = default;
        [[nodiscard]] virtual auto GetPointDistanceInMinutes(int index) -> int = 0;
        [[nodiscard]] virtual auto GetPointsAssignedIndex() -> int = 0;
        [[nodiscard]] virtual auto GetPointsCalculatedIndex() -> int = 0;
        [[nodiscard]] virtual auto GetPointsNumber() -> int = 0;
        [[nodiscard]] virtual auto GetPointName(int index) -> const char* = 0;
        [[nodiscard]] virtual auto GetPointPosition(int index) -> EuroScopePlugIn::CPosition = 0;

        // The value returned by GetPointDistanceInMinutes if the point is passed.
        const int pointPassed = -1;

        // The value returned by GetPointsAssignedIndex if no direct given.
        const int noDirect = -1;
    };
} // namespace UKControllerPlugin::Euroscope
