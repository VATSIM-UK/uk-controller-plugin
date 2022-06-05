#include "DrawTriangle.h"
#include "graphics/GdiGraphicsInterface.h"

namespace UKControllerPlugin::Graphics {

    // Coordinates of the triangle
    Gdiplus::Point trianglePoints[3] = {Gdiplus::Point(0, -5), Gdiplus::Point(5, 5), Gdiplus::Point(-5, 5)};

    // The default draw size of a triangle
    Gdiplus::REAL triangleSize(10);

    void FillTriangle(Windows::GdiGraphicsInterface& graphics, Gdiplus::Brush& brush, const Gdiplus::Rect& drawArea)
    {
        Gdiplus::REAL scaleX = drawArea.Width / triangleSize;
        Gdiplus::REAL scaleY = drawArea.Height / triangleSize;

        graphics.Scaled(scaleX, scaleY, [&graphics, &brush]() { graphics.FillPolygon(trianglePoints, brush, 3); });
    }

    auto TriangleDrawDimensions() -> Gdiplus::REAL
    {
        return triangleSize;
    }
} // namespace UKControllerPlugin::Graphics
