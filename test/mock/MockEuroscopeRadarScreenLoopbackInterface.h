#pragma once
#include "euroscope/EuroscopeRadarLoopbackInterface.h"
#include "plugin/PopupMenuItem.h"

namespace UKControllerPluginTest {
    namespace Euroscope {

        class MockEuroscopeRadarScreenLoopbackInterface :
            public UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface
        {
            public:
                MOCK_METHOD1(AddMenuItem, void(UKControllerPlugin::Plugin::PopupMenuItem menuItem));
                MOCK_METHOD0(GetRadarViewport, RECT(void));
                MOCK_METHOD4(RegisterScreenObject, void(int, std::string, RECT, bool));
                MOCK_METHOD1(GetAsrData, std::string(std::string));
                MOCK_METHOD1(GetGroundspeedForCallsign, int(std::string));
                MOCK_METHOD1(HasAsrKey, bool(std::string));
                MOCK_METHOD1(PositionOffScreen, bool(EuroScopePlugIn::CPosition));
                MOCK_METHOD3(ToogleMenu, void(RECT, std::string, int));
                MOCK_METHOD1(ConvertCoordinateToScreenPoint, POINT(EuroScopePlugIn::CPosition));
        };
    }  // namespace Euroscope
}  // namespace UKControllerPluginTest
