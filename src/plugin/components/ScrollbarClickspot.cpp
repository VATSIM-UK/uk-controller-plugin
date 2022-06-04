#include "ClickableArea.h"
#include "ScrollbarClickspot.h"
#include "StandardButtons.h"
#include "graphics/GdiGraphicsInterface.h"

namespace UKControllerPlugin::Components {

    ScrollbarClickspot::ScrollbarClickspot(int screenObjectId, std::function<void()> clicked, bool increments)
        : clicked(std::move(std::move(clicked))), increments(increments),
          clickableArea(ClickableArea::Create({}, screenObjectId, "scrollbarClickspot", false))
    {
    }

    void ScrollbarClickspot::Draw(
        const Gdiplus::Rect& area,
        Windows::GdiGraphicsInterface& graphics,
        Euroscope::EuroscopeRadarLoopbackInterface& radarScreen)
    {
        graphics.Translated(area.GetLeft(), area.GetTop(), [this, &graphics, &area] {
            CollapseButton([this]() { return increments; })(graphics, {0, 0, area.Width, area.Height});
        });
        clickableArea->WithPosition(area);
        clickableArea->Apply(graphics, radarScreen);
    }

    void ScrollbarClickspot::LeftClick(
        Euroscope::EuroscopeRadarLoopbackInterface& radarScreen, const std::string& description, POINT mousePosition)
    {
        clicked();
    }
} // namespace UKControllerPlugin::Components
