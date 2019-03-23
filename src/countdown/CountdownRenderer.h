#pragma once
#include "radarscreen/ConfigurableDisplayInterface.h"
#include "radarscreen/RadarRenderableInterface.h"
#include "euroscope/AsrEventHandlerInterface.h"
#include "plugin/PopupMenuItem.h"

// Forward declarations
namespace UKControllerPlugin {
    namespace Countdown {
        class CountdownTimer;
    }  // namespace Countdown
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Euroscope {
        class UserSetting;
        class EuroscopeRadarLoopbackInterface;
    }  // namespace Euroscope

    namespace Windows {
        class GdiGraphicsInterface;
        struct GdiplusBrushes;
    }  // namespace Windows
}  // namespace UKControllerPlugin

namespace Gdiplus {
    class Brush;
}  // namespace Gdiplus
// END

namespace UKControllerPlugin {
    namespace Countdown {

        /*
            A class for rendering the Countdown module.
        */
        class CountdownRenderer :
            public UKControllerPlugin::RadarScreen::ConfigurableDisplayInterface,
            public UKControllerPlugin::RadarScreen::RadarRenderableInterface,
            public UKControllerPlugin::Euroscope::AsrEventHandlerInterface
        {
            public:
                CountdownRenderer(
                    UKControllerPlugin::Countdown::CountdownTimer & countdownModule,
                    int functionsClickspotId,
                    int timeDisplayClickspotId,
                    int closeClickspotId,
                    int toogleCallbackFunctionId,
                    const UKControllerPlugin::Windows::GdiplusBrushes & brushes
                );
                void AsrLoadedEvent(UKControllerPlugin::Euroscope::UserSetting & userSetting) override;
                void AsrClosingEvent(UKControllerPlugin::Euroscope::UserSetting & userSetting) override;
                void Configure(int functionId, std::string subject) override;
                UKControllerPlugin::Plugin::PopupMenuItem GetConfigurationMenuItem(void) const override;
                RECT GetCloseClickspotDisplayArea(void) const;
                RECT GetNinetySecondDisplayArea(void) const;
                RECT GetOneMinuteDisplayArea(void) const;
                RECT GetResetDisplayArea(void) const;
                RECT GetThirtySecondDisplayArea(void) const;
                RECT GetThreeMinuteDisplayArea(void) const;
                RECT GetTimeDisplayArea(void) const;
                RECT GetTwoMinuteDisplayArea(void) const;
                bool IsVisible(void) const override;
                void LeftClick(
                    int objectId,
                    std::string objectDescription,
                    UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface & radarScreen
                ) override;
                void Move(RECT position, std::string objectDescription) override;
                void Render(
                    UKControllerPlugin::Windows::GdiGraphicsInterface & graphics,
                    UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface & radarScreen
                ) override;
                void RightClick(
                    int objectId,
                    std::string objectDescription,
                    UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface & radarScreen
                ) override;
                void SetVisible(bool visible);

                // The clickspot id for the number buttons and the reset button
                const int functionsClickspotId;

                // The clickspot id for the time display
                const int timeDisplayClickspotId;

                // The clickspot id for the close clickspot
                const int closeClickspotId;

                // The function ID for the toggle callback
                const int toggleCallbackFunctionId;

                // The ASR key for screen pos X
                const std::string userSettingPosX = "CountdownScreenPosX";

                // The ASR key for screen pos Y
                const std::string userSettingPosY = "CountdownScreenPosY";

                // The ASR key for displaying
                const std::string userSettingVisible = "DisplayCountdown";

                // The height of each row
                const int rowHeight = 20;

                // The width of the time display
                const int timeDisplayWidth = 150;

                // The width of each button
                const int buttonWidth = 30;

                // The description when saving the visibility ASR setting
                const std::string visibleUserSettingDescription = "Display UKCP Countdown Timer";

                // The description when saving the X position ASR setting
                const std::string xPositionUserSettingDescription = "Countdown X Position";

                // The description when saving the Y position ASR setting
                const std::string yPositionUserSettingDescription = "Countdown Y Position";

                // The countdown module menu text
                const std::string menuItemDescription = "Display Countdown Timer";

            private:
                void FunctionButtonClick(
                    std::string function,
                    UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface & radarScreen
                );
                std::wstring GetCurrentTimeString(int secondsRemaining);
                const Gdiplus::Brush & CountdownRenderer::GetTimeColour(int secondsRemaining);
                void RenderButtons(
                    UKControllerPlugin::Windows::GdiGraphicsInterface & graphics,
                    UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface & radarScreen
                );
                void RenderTimeDisplay(
                    UKControllerPlugin::Windows::GdiGraphicsInterface & graphics,
                    UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface & radarScreen
                );
                void ShiftAllElements(int topLeftX, int topLeftY);

                // The area for displaying the time
                RECT timeDisplayArea;

                // The area for displaying the close clickspot
                RECT closeClickspotDisplayArea;

                // The area for displaying the thirty second clickspot
                RECT thirtySecondDisplayArea;

                // The area for displaying the one minute button
                RECT oneMinuteDisplayArea;

                // The area for displaying the ninety second clickspot
                RECT ninetysecondDisplayArea;

                // The area for displaying the two minute button
                RECT twoMinuteDisplayArea;

                // The area for displaying the three minute button
                RECT threeMinuteDisplayArea;

                // The area for displaying the reset button
                RECT resetDisplayArea;

                // Whether or not the module should be visible on the screen.
                bool visible;

                // The countdown module that we're rendering
                UKControllerPlugin::Countdown::CountdownTimer & countdownModule;

                // A set of brushes to use for rendering.
                const UKControllerPlugin::Windows::GdiplusBrushes & brushes;
        };
    }  // namespace Countdown
}  // namespace UKControllerPlugin
