#pragma once
#include "radarscreen/RadarRenderableInterface.h"
#include "euroscope/EuroscopeRadarLoopbackInterface.h"
#include "graphics/GdiGraphicsInterface.h"

namespace UKControllerPluginTest {
    namespace RadarScreen {

        class MockRadarRenderableInterface : public UKControllerPlugin::RadarScreen::RadarRenderableInterface
        {
            public:
            MOCK_CONST_METHOD0(IsVisible, bool(void));
            MOCK_METHOD5(
                LeftClick,
                void(
                    UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface&,
                    int,
                    const std::string&,
                    POINT,
                    RECT));
            MOCK_METHOD3(
                RightClick,
                void(int, const std::string&, UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface&));
            MOCK_METHOD2(Move, void(RECT, std::string));
            MOCK_METHOD2(
                Render,
                void(
                    UKControllerPlugin::Windows::GdiGraphicsInterface&,
                    UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface&));
            MOCK_METHOD0(ResetPosition, void(void));
        };
    } // namespace RadarScreen
} // namespace UKControllerPluginTest
