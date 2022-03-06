#pragma once
#include <string>
#include <windef.h>
#include "radarscreen/RadarRenderableInterface.h"

namespace UKControllerPlugin {
    namespace Components {
        class TitleBar;
    } // namespace Components
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Approach {
    class ApproachSequencerDisplayOptions;

    /**
     * Displays the approach sequencer.
     */
    class ApproachSequencerDisplay : public RadarScreen::RadarRenderableInterface
    {
        public:
        ApproachSequencerDisplay(std::shared_ptr<ApproachSequencerDisplayOptions> displayOptions, int screenObjectId);
        auto IsVisible() const -> bool override;
        void LeftClick(
            Euroscope::EuroscopeRadarLoopbackInterface& radarScreen,
            int objectId,
            const std::string& objectDescription,
            POINT mousePos,
            RECT itemArea) override;
        void Move(RECT position, std::string objectDescription) override;
        void Render(
            Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen) override;
        void ResetPosition() override;
        [[nodiscard]] auto ContentCollapsed() const -> bool;

        private:
        void RenderAirfield(Windows::GdiGraphicsInterface& graphics);

        inline static const int WINDOW_WIDTH = 435;
        inline static const int TITLE_BAR_HEIGHT = 20;
        inline static const int CONTENT_HEIGHT = 150;
        inline static const int INSETS = 5;

        std::shared_ptr<ApproachSequencerDisplayOptions> displayOptions;

        POINT windowPosition{200, 200};

        Gdiplus::Rect titleBarArea = {0, 0, WINDOW_WIDTH, TITLE_BAR_HEIGHT};
        Gdiplus::Rect contentArea = {0, TITLE_BAR_HEIGHT, WINDOW_WIDTH, CONTENT_HEIGHT};
        Gdiplus::Rect airfieldStaticArea = {INSETS, TITLE_BAR_HEIGHT + INSETS, 75, TITLE_BAR_HEIGHT};
        Gdiplus::Rect airfieldTextArea = {
            airfieldStaticArea.GetRight() + INSETS, airfieldStaticArea.GetTop(), 40, TITLE_BAR_HEIGHT};

        std::shared_ptr<Components::TitleBar> titleBar;

        const Gdiplus::Color BACKGROUND_COLOUR = Gdiplus::Color(64, 64, 64);
        const Gdiplus::Color TEXT_COLOUR = Gdiplus::Color(225, 225, 225);
        std::shared_ptr<Gdiplus::Brush> backgroundBrush;
        std::shared_ptr<Gdiplus::Brush> textBrush;

        bool contentCollapsed = false;
        bool isVisible = true;
    };
} // namespace UKControllerPlugin::Approach
