#include "StandardButtons.h"
#include "graphics/GdiGraphicsInterface.h"
#include "theme/ThemeManager.h"

using UKControllerPlugin::Theme::PaletteKey;
using UKControllerPlugin::Theme::ThemeManager;

namespace UKControllerPlugin::Components {

    // Coordinates of the close cross
    Gdiplus::Point closeTopLeft = {-5, -5};
    Gdiplus::Point closeBottomRight = {5, 5};
    Gdiplus::Point closeBottomLeft = {-5, 5};
    Gdiplus::Point closeTopRight = {5, -5};

    // Coordinates of the collapse triangle
    Gdiplus::Point collapsePoints[3] = {Gdiplus::Point(0, -5), Gdiplus::Point(5, 5), Gdiplus::Point(-5, 5)};

    // The standard button sizing
    Gdiplus::REAL buttonSize(15);

    /*
     * Draws a standard X-shaped close button of the specified colour.
     */
    auto CloseButton(PaletteKey key) -> std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)>
    {
        auto pen = std::make_shared<Gdiplus::Pen>(Gdiplus::Color(), 2.0F);
        return [key, pen](Windows::GdiGraphicsInterface& graphics, const Gdiplus::Rect& drawArea) {
            pen->SetColor(ThemeManager::Colour(key));

            Gdiplus::REAL scaleX = drawArea.Width / buttonSize;
            Gdiplus::REAL scaleY = drawArea.Height / buttonSize;
            ScalePen(pen, scaleX, scaleY);

            graphics.Translated(
                static_cast<Gdiplus::REAL>(5) * scaleX,
                static_cast<Gdiplus::REAL>(5) * scaleY,
                [&graphics, &pen, &scaleX, &scaleY] {
                    graphics.Scaled(scaleX, scaleY, [&graphics, &pen]() {
                        graphics.DrawLine(*pen, closeTopLeft, closeBottomRight);
                        graphics.DrawLine(*pen, closeBottomLeft, closeTopRight);
                    });
                });
        };
    }

    std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)> CloseButton()
    {
        return CloseButton(PaletteKey::Text);
    }

    /*
     * Draws a standard triangle shaped collapse button of the specified colour.
     *
     * The state function tells the button whether it is collapsed or not.
     */
    std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)>
    CollapseButton(PaletteKey key, std::function<bool()> stateFunction)
    {
        return [key, stateFunction](Windows::GdiGraphicsInterface& graphics, const Gdiplus::Rect& drawArea) {
            const auto& brush = ThemeManager::Brush(key);

            Gdiplus::REAL scaleX = drawArea.Width / buttonSize;
            Gdiplus::REAL scaleY = drawArea.Height / buttonSize;

            graphics.Translated(
                static_cast<Gdiplus::REAL>(5) * scaleX,
                static_cast<Gdiplus::REAL>(5) * scaleY,
                [&graphics, &stateFunction, &brush, &scaleX, &scaleY] {
                    graphics.Rotated(
                        stateFunction() ? static_cast<Gdiplus::REAL>(180) : static_cast<Gdiplus::REAL>(0),
                        [&graphics, &brush, &scaleX, &scaleY]() {
                            graphics.Scaled(scaleX, scaleY, [&graphics, &brush]() {
                                graphics.FillPolygon(collapsePoints, brush, 3);
                            });
                        });
                });
        };
    }

    std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)>
    CollapseButton(std::function<bool()> stateFunction)
    {
        return CollapseButton(PaletteKey::Text, std::move(stateFunction));
    }

    void ScalePen(const std::shared_ptr<Gdiplus::Pen>& pen, Gdiplus::REAL& scaleX, Gdiplus::REAL& scaleY)
    {
        pen->ResetTransform();
        pen->ScaleTransform(1 / scaleX, 1 / scaleY);
    }

    std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)> DeleteButton(PaletteKey key)
    {
        return CloseButton(key);
    }

    std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)> DeleteButton()
    {
        return CloseButton();
    }

    std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)> UpArrow(PaletteKey key)
    {
        return CollapseButton(key, []() { return false; });
    }

    std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)> UpArrow()
    {
        return CollapseButton([]() { return false; });
    }

    std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)> DownArrow(PaletteKey key)
    {
        return CollapseButton(key, []() { return true; });
    }

    std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)> DownArrow()
    {
        return CollapseButton([]() { return true; });
    }
} // namespace UKControllerPlugin::Components
