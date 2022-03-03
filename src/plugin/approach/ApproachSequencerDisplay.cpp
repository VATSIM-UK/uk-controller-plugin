#include "ApproachSequencerDisplay.h"
#include "components/CollapsibleWindowTitleBar.h"
#include "graphics/GdiGraphicsInterface.h"

using UKControllerPlugin::Components::CollapsibleWindowTitleBar;

namespace UKControllerPlugin::Approach {

    ApproachSequencerDisplay::ApproachSequencerDisplay(int screenObjectId)
        : titleBar(CollapsibleWindowTitleBar::Create(
              L"Approach Sequencer", titleBarArea, [this]() -> bool { return this->contentCollapsed; }, screenObjectId))
    {
    }

    auto ApproachSequencerDisplay::IsVisible() const -> bool
    {
        return true;
    }

    void ApproachSequencerDisplay::Move(RECT position, std::string objectDescription)
    {
        this->windowPosition = {position.left, position.top};
    }

    void ApproachSequencerDisplay::Render(
        Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen)
    {
        graphics.Translated(windowPosition.x, windowPosition.y, [this, &graphics, &radarScreen]() {
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
        RadarRenderableInterface::LeftClick(radarScreen, objectId, objectDescription, mousePos, itemArea);
    }
} // namespace UKControllerPlugin::Approach
// namespace UKControllerPlugin::Approach
