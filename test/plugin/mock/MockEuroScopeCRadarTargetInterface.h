#pragma once
#include "euroscope/EuroScopeCRadarTargetInterface.h"

namespace UKControllerPluginTest {
    namespace Euroscope {

        class MockEuroScopeCRadarTargetInterface : public UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface
        {
            public:
                MOCK_CONST_METHOD0(GetCallsign, const std::string(void));
                MOCK_CONST_METHOD0(GetFlightLevel, int(void));
                MOCK_CONST_METHOD0(GetGroundSpeed, const int(void));
                MOCK_CONST_METHOD0(GetPosition, const EuroScopePlugIn::CPosition(void));
                MOCK_CONST_METHOD0(GetVerticalSpeed, int(void));
                MOCK_METHOD(double, GetHeading, (void), (const, override));
        };
    }  // namespace Euroscope
}  // namespace UKControllerPluginTest
