#pragma once
#include "euroscope/EuroscopeExtractedRouteInterface.h"

namespace UKControllerPluginTest::Euroscope {
    class MockEuroscopeExtractedRouteInterface : public UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface
    {
        public:
        MockEuroscopeExtractedRouteInterface();
        virtual ~MockEuroscopeExtractedRouteInterface();
        MOCK_METHOD0(GetPointsNumber, int(void));
        MOCK_METHOD1(GetPointName, const char*(int));
        MOCK_METHOD1(GetPointPosition, EuroScopePlugIn::CPosition(int));
        MOCK_METHOD1(GetPointDistanceInMinutes, int(int));
        MOCK_METHOD0(GetPointsAssignedIndex, int(void));
        MOCK_METHOD0(GetPointsCalculatedIndex, int(void));
    };
} // namespace UKControllerPluginTest::Euroscope
