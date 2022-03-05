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
        POINT windowPosition{200, 200};

        Gdiplus::Rect titleBarArea = {0, 0, 435, 20};

        std::shared_ptr<Components::TitleBar> titleBar;

        bool contentCollapsed = false;
    };
} // namespace UKControllerPlugin::Approach
