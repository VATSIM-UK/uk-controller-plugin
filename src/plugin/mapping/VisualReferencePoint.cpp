#include "VisualReferencePoint.h"
#include "euroscope/EuroscopeRadarLoopbackInterface.h"
#include "graphics/GdiGraphicsInterface.h"

namespace UKControllerPlugin::Mapping {

    VisualReferencePoint::VisualReferencePoint(std::string label, EuroScopePlugIn::CPosition position)
        : label(label), position(std::move(position)), pen(std::make_shared<Gdiplus::Pen>(Gdiplus::Color(50, 127, 168)))
    {
    }

    void VisualReferencePoint::Draw(
        Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen)
    {
        const auto screenPosition = radarScreen.ConvertCoordinateToScreenPoint(position);
        Gdiplus::Rect area{screenPosition.x - 20, screenPosition.y - 20, 20, 20};

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

    auto VisualReferencePoint::Position() const -> const EuroScopePlugIn::CPosition&
    {
        return position;
    }

    auto VisualReferencePoint::Label() const -> const std::string&
    {
        return label;
    }
} // namespace UKControllerPlugin::Mapping
