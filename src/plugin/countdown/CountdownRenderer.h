#pragma once
#include "euroscope/AsrEventHandlerInterface.h"
#include "plugin/PopupMenuItem.h"
#include "radarscreen/ConfigurableDisplayInterface.h"
#include "radarscreen/RadarRenderableInterface.h"

namespace UKControllerPlugin {
    namespace Euroscope {
        class UserSetting;
        class EuroscopeRadarLoopbackInterface;
    } // namespace Euroscope

    namespace Windows {
        class GdiGraphicsInterface;
        struct GdiplusBrushes;
    } // namespace Windows
} // namespace UKControllerPlugin

namespace Gdiplus {
    class Brush;
} // namespace Gdiplus

namespace UKControllerPlugin::Countdown {

    class CountdownTimer;
    class TimerConfigurationManager;

    /*
        A class for rendering the Countdown module.
    */
    class CountdownRenderer : public UKControllerPlugin::RadarScreen::ConfigurableDisplayInterface,
                              public UKControllerPlugin::RadarScreen::RadarRenderableInterface,
                              public UKControllerPlugin::Euroscope::AsrEventHandlerInterface
    {
        public:
        CountdownRenderer(
            UKControllerPlugin::Countdown::CountdownTimer& countdownModule,
            const UKControllerPlugin::Countdown::TimerConfigurationManager& configManager,
            int functionsClickspotId,
            int timeDisplayClickspotId,
            int closeClickspotId,
            int toogleCallbackFunctionId,
            const UKControllerPlugin::Windows::GdiplusBrushes& brushes);
        void AsrLoadedEvent(UKControllerPlugin::Euroscope::UserSetting& userSetting) override;
        void AsrClosingEvent(UKControllerPlugin::Euroscope::UserSetting& userSetting) override;
        void Configure(int functionId, std::string subject, RECT screenObjectArea) override;
        [[nodiscard]] auto GetConfigurationMenuItem() const -> UKControllerPlugin::Plugin::PopupMenuItem override;
        [[nodiscard]] auto GetTimerButtonArea(unsigned int configId) const -> RECT;
        [[nodiscard]] auto GetResetDisplayArea() const -> RECT;
        [[nodiscard]] auto GetCloseClickspotDisplayArea() const -> RECT;
        [[nodiscard]] auto GetTimeDisplayArea() const -> RECT;
        [[nodiscard]] auto IsVisible() const -> bool override;
        void LeftClick(
            UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface& radarScreen,
            int objectId,
            const std::string& objectDescription,
            POINT mousePos,
            RECT itemArea) override;
        void Move(RECT position, std::string objectDescription) override;
        void Render(
            UKControllerPlugin::Windows::GdiGraphicsInterface& graphics,
            UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface& radarScreen) override;
        void SetVisible(bool visible);
        void ResetPosition() override;

        private:
        void FunctionButtonClick(
            const std::string& function, UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface& radarScreen);
        static auto GetCurrentTimeString(int secondsRemaining) -> std::wstring;
        auto GetTimeColour(int secondsRemaining) -> const Gdiplus::Brush&;
        void RenderButtons(
            UKControllerPlugin::Windows::GdiGraphicsInterface& graphics,
            UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface& radarScreen);
        void RenderTimeDisplay(
            UKControllerPlugin::Windows::GdiGraphicsInterface& graphics,
            UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface& radarScreen);
        void ShiftAllElements(int topLeftX, int topLeftY);

        // The area for displaying the time
        RECT timeDisplayArea;

        // The area for displaying the close clickspot
        RECT closeClickspotDisplayArea;

        // The area for displaying the reset button
        RECT resetDisplayArea;

        // The display areas of each timer configuration
        std::map<unsigned int, RECT> timerButtonAreas;

        // Whether or not the module should be visible on the screen.
        bool visible;

        // The countdown module that we're rendering
        UKControllerPlugin::Countdown::CountdownTimer& countdownModule;

        // A set of brushes to use for rendering.
        const UKControllerPlugin::Windows::GdiplusBrushes& brushes;

        // Handles the timer configurations
        const UKControllerPlugin::Countdown::TimerConfigurationManager& configManager;

        // The last version of the config
        unsigned int lastConfigVersion = 0;

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

        // The width of each button
        const int buttonWidth = 35;

        // The description when saving the visibility ASR setting
        const std::string visibleUserSettingDescription = "Display UKCP Countdown Timer";

        // The description when saving the X position ASR setting
        const std::string xPositionUserSettingDescription = "Countdown X Position";

        // The description when saving the Y position ASR setting
        const std::string yPositionUserSettingDescription = "Countdown Y Position";

        // The countdown module menu text
        const std::string menuItemDescription = "Display Countdown Timer";

        inline static const int DEFAULT_POSITION = 100;

        inline static const int SECONDS_IN_MINUTE = 60;

        inline static const int TEN_SECONDS = 10;
        inline static const int WARNING_SECONDS = 30;
        inline static const int CRITICAL_SECONDS = 10;
    };
} // namespace UKControllerPlugin::Countdown
