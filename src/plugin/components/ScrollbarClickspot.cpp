#include "ClickableArea.h"
#include "ScrollbarClickspot.h"
#include "graphics/DrawTriangle.h"
#include "graphics/GdiGraphicsInterface.h"

namespace UKControllerPlugin::Components {

    ScrollbarClickspot::ScrollbarClickspot(int screenObjectId, std::function<void()> clicked, bool increments)
        : clicked(std::move(std::move(clicked))), increments(increments),
          clickableArea(ClickableArea::Create({}, screenObjectId, "scrollbarClickspot", false)),
          brush(std::make_shared<Gdiplus::SolidBrush>(Gdiplus::Color(255, 255, 255)))
    {
    }

    void ScrollbarClickspot::Draw(
        const Gdiplus::Rect& area,
        Windows::GdiGraphicsInterface& graphics,
        Euroscope::EuroscopeRadarLoopbackInterface& radarScreen)
    {
        if (horizontal) {
        }
        graphics.Translated(
            Graphics::TriangleDrawDimensions(), Graphics::TriangleDrawDimensions(), [this, &graphics, &area] {
                graphics.Rotated(Gdiplus::REAL((increments ? 180 : 0)), [this, &graphics, &area] {
                    Graphics::FillTriangle(graphics, *brush, area);
                });
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
