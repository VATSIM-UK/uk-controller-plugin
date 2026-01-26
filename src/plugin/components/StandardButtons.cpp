#include "StandardButtons.h"
#include "graphics/GdiGraphicsInterface.h"
#include "graphics/GdiplusBrushes.h"

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
    auto CloseButton(Gdiplus::Color colour) -> std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)>
    {
        auto pen = std::make_shared<Gdiplus::Pen>(colour, 2.0F);
        return [pen](Windows::GdiGraphicsInterface& graphics, const Gdiplus::Rect& drawArea) {
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

    std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)>
    CloseButton(const Windows::GdiplusBrushes& brushes)
    {
        return [&brushes](Windows::GdiGraphicsInterface& graphics, const Gdiplus::Rect& drawArea) {
            // Create pen at draw time so it uses the current brush color
            Gdiplus::Pen pen(brushes.text, 2.0F);
            Gdiplus::REAL scaleX = drawArea.Width / buttonSize;
            Gdiplus::REAL scaleY = drawArea.Height / buttonSize;
            pen.ResetTransform();
            pen.ScaleTransform(1 / scaleX, 1 / scaleY);

            graphics.Translated(
                static_cast<Gdiplus::REAL>(5) * scaleX,
                static_cast<Gdiplus::REAL>(5) * scaleY,
                [&graphics, &pen, &scaleX, &scaleY] {
                    graphics.Scaled(scaleX, scaleY, [&graphics, &pen]() {
                        graphics.DrawLine(pen, closeTopLeft, closeBottomRight);
                        graphics.DrawLine(pen, closeBottomLeft, closeTopRight);
                    });
                });
        };
    }

    /*
     * Draws a standard triangle shaped collapse button of the specified colour.
     *
     * The state function tells the button whether it is collapsed or not.
     */
    std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)>
    CollapseButton(Gdiplus::Color colour, std::function<bool()> stateFunction)
    {
        auto brush = std::make_shared<Gdiplus::SolidBrush>(colour);
        return [brush, stateFunction](Windows::GdiGraphicsInterface& graphics, const Gdiplus::Rect& drawArea) {
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
                                graphics.FillPolygon(collapsePoints, *brush, 3);
                            });
                        });
                });
        };
    }

    std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)>
    CollapseButton(const Windows::GdiplusBrushes& brushes, std::function<bool()> stateFunction)
    {
        return [&brushes, stateFunction](Windows::GdiGraphicsInterface& graphics, const Gdiplus::Rect& drawArea) {
            // Create brush at draw time so it uses the current brush color
            Gdiplus::SolidBrush brush(brushes.text);
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

    void ScalePen(const std::shared_ptr<Gdiplus::Pen>& pen, Gdiplus::REAL& scaleX, Gdiplus::REAL& scaleY)
    {
        pen->ResetTransform();
        pen->ScaleTransform(1 / scaleX, 1 / scaleY);
    }

    std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)> DeleteButton(Gdiplus::Color colour)
    {
        return CloseButton(colour);
    }

    std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)>
    DeleteButton(const Windows::GdiplusBrushes& brushes)
    {
        return CloseButton(brushes);
    }

    std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)> UpArrow(Gdiplus::Color colour)
    {
        return CollapseButton(colour, []() { return false; });
    }

    std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)>
    UpArrow(const Windows::GdiplusBrushes& brushes)
    {
        return CollapseButton(brushes, []() { return false; });
    }

    std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)> DownArrow(Gdiplus::Color colour)
    {
        return CollapseButton(colour, []() { return true; });
    }

    std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)>
    DownArrow(const Windows::GdiplusBrushes& brushes)
    {
        return CollapseButton(brushes, []() { return true; });
    }
} // namespace UKControllerPlugin::Components
