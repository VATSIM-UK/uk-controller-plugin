#include "ApproachSequencerDisplay.h"
#include "components/CollapsibleWindowTitleBar.h"
#include "graphics/GdiGraphicsInterface.h"

using UKControllerPlugin::Components::CollapsibleWindowTitleBar;

namespace UKControllerPlugin::Approach {

    ApproachSequencerDisplay::ApproachSequencerDisplay(int screenObjectId)
        : titleBar(CollapsibleWindowTitleBar::Create(
              L"Approach Sequencer",
              titleBarArea,
              [this]() -> bool { return this->contentCollapsed; },
              screenObjectId)),
          backgroundBrush(std::make_shared<Gdiplus::SolidBrush>(BACKGROUND_COLOUR))
    {
    }

    auto ApproachSequencerDisplay::IsVisible() const -> bool
    {
        return this->isVisible;
    }

    void ApproachSequencerDisplay::Move(RECT position, std::string objectDescription)
    {
        this->windowPosition = {position.left, position.top};
    }

    void ApproachSequencerDisplay::Render(
        Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen)
    {
        graphics.Translated(windowPosition.x, windowPosition.y, [this, &graphics, &radarScreen]() {
            if (this->contentCollapsed) {
                this->titleBar->Draw(graphics, radarScreen);
                return;
            }

            graphics.FillRect(contentArea, *backgroundBrush);
            this->titleBar->Draw(graphics, radarScreen);
        });
    }

    void ApproachSequencerDisplay::ResetPosition()
    {
        RadarRenderableInterface::ResetPosition();
    }

    void ApproachSequencerDisplay::LeftClick(
        Euroscope::EuroscopeRadarLoopbackInterface& radarScreen,
        int objectId,
        const std::string& objectDescription,
        POINT mousePos,
        RECT itemArea)
    {
        if (objectDescription == "collapseButton") {
            this->contentCollapsed = !this->contentCollapsed;
        }

        if (objectDescription == "closeButton") {
            this->isVisible = false;
        }
    }

    auto ApproachSequencerDisplay::ContentCollapsed() const -> bool
    {
        return contentCollapsed;
    }
} // namespace UKControllerPlugin::Approach
