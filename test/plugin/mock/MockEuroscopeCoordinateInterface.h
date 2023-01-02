#pragma once
#include "euroscope/EuroscopeCoordinateInterface.h"

namespace UKControllerPluginTest::Euroscope {
    class MockEuroscopeCoordinateInterface : public UKControllerPlugin::Euroscope::EuroscopeCoordinateInterface
    {
        public:
        MOCK_METHOD(double, DirectionTo, (const EuroscopeCoordinateInterface&), (const, override));
        MOCK_METHOD(const EuroScopePlugIn::CPosition, ToEuroscopePosition, (), (const, override));
    };
} // namespace UKControllerPluginTest::Euroscope
