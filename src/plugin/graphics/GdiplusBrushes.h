#pragma once

namespace UKControllerPlugin {
namespace Windows {

typedef struct GdiplusBrushes {
    const std::unique_ptr<const Gdiplus::Brush> whiteBrush = std::make_unique<Gdiplus::SolidBrush>(
        Gdiplus::Color(255, 255, 255, 255)
    );
    const std::unique_ptr<const Gdiplus::Brush> yellowBrush = std::make_unique<Gdiplus::SolidBrush>(
        Gdiplus::Color(255, 255, 255, 0)
    );
    const std::unique_ptr<const Gdiplus::Brush> euroscopeBackgroundBrush = std::make_unique<Gdiplus::SolidBrush>(
        Gdiplus::Color(255, 11, 65, 54)
    );
    const std::unique_ptr<const Gdiplus::Brush> blackBrush = std::make_unique<Gdiplus::SolidBrush>(
        Gdiplus::Color(255, 0, 0, 0)
    );
    const std::unique_ptr<const Gdiplus::Pen> blackPen = std::make_unique<Gdiplus::Pen>(
        Gdiplus::Color(255, 0, 0, 0)
    );
    const std::unique_ptr<const Gdiplus::Brush> greyBrush = std::make_unique<Gdiplus::SolidBrush>(
        Gdiplus::Color(255, 125, 125, 125)
    );
    const std::unique_ptr<const Gdiplus::Brush> redBrush = std::make_unique<Gdiplus::SolidBrush>(
        Gdiplus::Color(255, 255, 0, 0)
    );
    const std::unique_ptr<const Gdiplus::Brush> greenBrush = std::make_unique<Gdiplus::SolidBrush>(
        Gdiplus::Color(255, 0, 255, 0)
    );
} GdiplusBrushes;

}  // namespace Windows
}  // namespace UKControllerPlugin
