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
                MOCK_METHOD3(
                    LeftClick,
                    void(int, std::string, UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface &)
                );
                MOCK_METHOD3(
                    RightClick,
                    void(int, std::string, UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface &)
                );
                MOCK_METHOD2(Move, void(RECT, std::string));
                MOCK_METHOD2(
                    Render,
                    void(
                        UKControllerPlugin::Windows::GdiGraphicsInterface &,
                        UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface &
                    )
                );
                MOCK_METHOD0(ResetPosition, void(void));
        };
    }  // namespace RadarScreen
}  // namespace UKControllerPluginTest
