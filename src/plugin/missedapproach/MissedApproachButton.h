#pragma once
#include "euroscope/AsrEventHandlerInterface.h"
#include "radarscreen/RadarRenderableInterface.h"

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
    } // namespace Euroscope
    namespace Ownership {
        class AirfieldServiceProviderCollection;
    } // namespace Ownership
} // namespace UKControllerPlugin

namespace UKControllerPlugin::MissedApproach {
    class MissedApproachCollection;
    class TriggerMissedApproach;

    /**
     * The button that can be pressed by the controller to trigger a missed
     * approach.
     */
    class MissedApproachButton : public Euroscope::AsrEventHandlerInterface,
                                 public RadarScreen::RadarRenderableInterface
    {
        public:
        MissedApproachButton(
            std::shared_ptr<const MissedApproachCollection> missedApproaches,
            std::shared_ptr<const TriggerMissedApproach> trigger,
            Euroscope::EuroscopePluginLoopbackInterface& plugin,
            const Ownership::AirfieldServiceProviderCollection& serviceProviders,
            int screenObjectId);

        void AsrLoadedEvent(Euroscope::UserSetting& userSetting) override;
        void AsrClosingEvent(Euroscope::UserSetting& userSetting) override;
        [[nodiscard]] auto IsVisible() const -> bool override;
        void LeftClick(
            Euroscope::EuroscopeRadarLoopbackInterface& radarScreen,
            int objectId,
            const std::string& objectDescription,
            POINT mousePos,
            RECT itemArea) override;
        void Move(RECT position, std::string objectDescription) override;
        [[nodiscard]] auto Position() const -> RECT;
        void Render(
            Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen) override;
        void ResetPosition() override;
        void ToggleVisible();

        private:
        void RenderActiveButton(Windows::GdiGraphicsInterface& graphics);
        void RenderArmedButton(Windows::GdiGraphicsInterface& graphics);
        void RenderDisabledButton(Windows::GdiGraphicsInterface& graphics);
        void RenderBorder(Windows::GdiGraphicsInterface& graphics);
        void RenderMissedApproachText(Windows::GdiGraphicsInterface& graphics);
        void RenderText(Windows::GdiGraphicsInterface& graphics, const std::wstring& text);
        void RenderBackground(Windows::GdiGraphicsInterface& graphics, const Gdiplus::Brush& brush);
        void ApplyClickspot(Euroscope::EuroscopeRadarLoopbackInterface& radarScreen);

        // All the missed appraoches so we know what button to render
        const std::shared_ptr<const MissedApproachCollection> missedApproaches;

        // Trigger mechanism
        const std::shared_ptr<const TriggerMissedApproach> trigger;

        // For getting flightplans
        Euroscope::EuroscopePluginLoopbackInterface& plugin;

        // Who's providing what service, where
        const Ownership::AirfieldServiceProviderCollection& serviceProviders;

        // The id of the button on screen
        const int screenObjectId;

        // Are we displaying the button
        bool isVisible;

        // ASR Keys
        const std::string ASR_VISIBILITY_KEY = "missedApproachButtonVisibility";
        const std::string ASR_VISIBILITY_DESC = "Missed Approach Button Visibility";
        const std::string ASR_X_POSITION_KEY = "missedApproachButtonXPosition";
        const std::string ASR_X_POSITION_DESC = "Missed Approach Button X Position";
        const std::string ASR_Y_POSITION_KEY = "missedApproachButtonYPosition";
        const std::string ASR_Y_POSITION_DESC = "Missed Approach Button Y Position";

        // Positional things
        static inline const int DEFAULT_COORDINATE = 200;
        static inline const int DEFAULT_WIDTH = 125;
        static inline const int DEFAULT_HEIGHT = 75;
        static inline const RECT DEFAULT_POSITION = {
            DEFAULT_COORDINATE,
            DEFAULT_COORDINATE,
            DEFAULT_COORDINATE + DEFAULT_WIDTH,
            DEFAULT_COORDINATE + DEFAULT_HEIGHT};
        RECT position;
        Gdiplus::Rect renderRect;

        // Drawing things
        const Gdiplus::REAL BORDER_THICKNESS = 3;
        const Gdiplus::Color BORDER_COLOR = Gdiplus::Color(255, 255, 255);
        const Gdiplus::Color ACTIVE_COLOR = Gdiplus::Color(179, 0, 0);
        const Gdiplus::Color ARMED_COLOR = Gdiplus::Color(0, 102, 20);
        const Gdiplus::Color DISABLED_COLOR = Gdiplus::Color(85, 85, 85);
        const Gdiplus::Color TEXT_COLOR = Gdiplus::Color(255, 255, 255);
        const Gdiplus::SolidBrush ACTIVE_BRUSH;
        const Gdiplus::SolidBrush DISABLED_BRUSH;
        const Gdiplus::SolidBrush ARMED_BRUSH;
        const Gdiplus::SolidBrush TEXT_BRUSH;
        const Gdiplus::Pen BORDER_PEN;
    };
} // namespace UKControllerPlugin::MissedApproach
