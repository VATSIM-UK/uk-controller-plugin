#pragma once

#include "theme/PaletteKey.h"

namespace UKControllerPlugin::Windows {
    class GdiGraphicsInterface;
} // namespace UKControllerPlugin::Windows

namespace UKControllerPlugin::Components {

    /*
     * Returns the draw function for a standard close button with a specified colour
     */
    std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)> CloseButton(Theme::PaletteKey key);
    std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)> CloseButton();

    /*
     * Returns the draw function for a standard delete button with a specified colour
     */
    std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)> DeleteButton(Theme::PaletteKey key);
    std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)> DeleteButton();

    /*
     * Returns the draw function for a standard collapse button with a specified colour.
     */
    std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)>
    CollapseButton(Theme::PaletteKey key, std::function<bool(void)> stateFunction);
    std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)>
    CollapseButton(std::function<bool(void)> stateFunction);

    /*
     * Returns the draw function for up and down arrows.
     */
    std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)> UpArrow(Theme::PaletteKey key);
    std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)> UpArrow();
    std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)> DownArrow(Theme::PaletteKey key);
    std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)> DownArrow();

    void ScalePen(const std::shared_ptr<Gdiplus::Pen>& pen, Gdiplus::REAL& scaleX, Gdiplus::REAL& scaleY);
} // namespace UKControllerPlugin::Components
