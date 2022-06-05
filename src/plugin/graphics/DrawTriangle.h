#pragma once

namespace UKControllerPlugin::Windows {
    class GdiGraphicsInterface;
} // namespace UKControllerPlugin::Windows

namespace UKControllerPlugin::Graphics {
    void FillTriangle(Windows::GdiGraphicsInterface& graphics, Gdiplus::Brush& brush, const Gdiplus::Rect& drawArea);
    [[nodiscard]] auto TriangleDrawDimensions() -> Gdiplus::REAL;
} // namespace UKControllerPlugin::Graphics
