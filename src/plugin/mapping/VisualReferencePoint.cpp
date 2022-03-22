#include "VisualReferencePoint.h"
#include "graphics/GdiGraphicsInterface.h"

namespace UKControllerPlugin::Mapping {

    VisualReferencePoint::VisualReferencePoint() : pen(std::make_shared<Gdiplus::Pen>(Gdiplus::Color(50, 127, 168)))
    {
    }

    void VisualReferencePoint::Draw(Windows::GdiGraphicsInterface& graphics, const Gdiplus::Rect& area)
    {
        graphics.DrawCircle(area, *pen);
        graphics.DrawLine(
            *pen,
            Gdiplus::PointF{static_cast<Gdiplus::REAL>(area.Width / 2), 0},
            Gdiplus::PointF{static_cast<Gdiplus::REAL>(area.Width / 2), static_cast<Gdiplus::REAL>(area.Height)});
        graphics.DrawLine(
            *pen,
            Gdiplus::PointF{0, static_cast<Gdiplus::REAL>(area.Height / 2)},
            Gdiplus::PointF{static_cast<Gdiplus::REAL>(area.Width), static_cast<Gdiplus::REAL>(area.Height / 2)});
    }
} // namespace UKControllerPlugin::Mapping
