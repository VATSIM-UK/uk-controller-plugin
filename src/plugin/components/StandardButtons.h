#pragma once

namespace UKControllerPlugin::Windows {
    class GdiGraphicsInterface;
    struct GdiplusBrushes;
} // namespace UKControllerPlugin::Windows

namespace UKControllerPlugin::Components {

    /*
     * Returns the draw function for a standard close button with a specified colour
     */
    std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)> CloseButton(Gdiplus::Color colour);
    std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)>
    CloseButton(const Windows::GdiplusBrushes& brushes);

    /*
     * Returns the draw function for a standard delete button with a specified colour
     */
    std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)> DeleteButton(Gdiplus::Color colour);
    std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)>
    DeleteButton(const Windows::GdiplusBrushes& brushes);

    /*
     * Returns the draw function for a standard collapse button with a specified colour.
     */
    std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)>
    CollapseButton(Gdiplus::Color colour, std::function<bool(void)> stateFunction);
    std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)>
    CollapseButton(const Windows::GdiplusBrushes& brushes, std::function<bool(void)> stateFunction);

    /*
     * Returns the draw function for up and down arrows.
     */
    std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)> UpArrow(Gdiplus::Color colour);
    std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)>
    UpArrow(const Windows::GdiplusBrushes& brushes);
    std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)> DownArrow(Gdiplus::Color colour);
    std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)>
    DownArrow(const Windows::GdiplusBrushes& brushes);

    void ScalePen(const std::shared_ptr<Gdiplus::Pen>& pen, Gdiplus::REAL& scaleX, Gdiplus::REAL& scaleY);
} // namespace UKControllerPlugin::Components
