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

    /**
     * Displays the approach sequencer.
     */
    class ApproachSequencerDisplay : public RadarScreen::RadarRenderableInterface
    {
        public:
        ApproachSequencerDisplay(int screenObjectId);
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
        inline static const int WINDOW_WIDTH = 435;
        inline static const int TITLE_BAR_HEIGHT = 20;
        inline static const int CONTENT_HEIGHT = 150;

        POINT windowPosition{200, 200};

        Gdiplus::Rect titleBarArea = {0, 0, WINDOW_WIDTH, TITLE_BAR_HEIGHT};
        Gdiplus::Rect contentArea = {0, TITLE_BAR_HEIGHT, WINDOW_WIDTH, CONTENT_HEIGHT};

        std::shared_ptr<Components::TitleBar> titleBar;

        const Gdiplus::Color BACKGROUND_COLOUR = Gdiplus::Color(64, 64, 64);
        const Gdiplus::Color TEXT_COLOUR = Gdiplus::Color(225, 225, 225);
        std::shared_ptr<Gdiplus::Brush> backgroundBrush;

        bool contentCollapsed = false;
        bool isVisible = true;
    };
} // namespace UKControllerPlugin::Approach
