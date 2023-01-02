#pragma once
#include "euroscope/EuroScopeCRadarTargetInterface.h"

namespace UKControllerPluginTest {
    namespace Euroscope {

        class MockEuroScopeCRadarTargetInterface : public UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface
        {
            public:
            MockEuroScopeCRadarTargetInterface();
            virtual ~MockEuroScopeCRadarTargetInterface();
            MOCK_CONST_METHOD0(GetCallsign, const std::string(void));
            MOCK_CONST_METHOD0(GetFlightLevel, int(void));
            MOCK_CONST_METHOD0(GetGroundSpeed, int(void));
            MOCK_CONST_METHOD0(GetPosition, const EuroScopePlugIn::CPosition(void));
            MOCK_CONST_METHOD0(GetVerticalSpeed, int(void));
            MOCK_METHOD(double, GetHeading, (), (const, override));
        };
    } // namespace Euroscope
} // namespace UKControllerPluginTest
