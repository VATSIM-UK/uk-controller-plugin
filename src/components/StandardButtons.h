#pragma once

namespace UKControllerPlugin::Windows {
    class GdiGraphicsInterface;
} // namespace UKControllerPlugin::Windows

namespace UKControllerPlugin::Components {

    /*
     * Returns the draw function for a standard close button with a specified colour
     */
    std::function<void(Windows::GdiGraphicsInterface& graphics)> CloseButton(Gdiplus::Color colour);
    std::function<void(Windows::GdiGraphicsInterface& graphics)> CloseButton();

    /*
     * Returns the draw function for a standard collapse button with a specified colour.
     */
    std::function<void(Windows::GdiGraphicsInterface& graphics)>
    CollapseButton(Gdiplus::Color colour, std::function<bool(void)> stateFunction);
    std::function<void(Windows::GdiGraphicsInterface& graphics)>
    CollapseButton(std::function<bool(void)> stateFunction);
} // namespace UKControllerPlugin::Components
