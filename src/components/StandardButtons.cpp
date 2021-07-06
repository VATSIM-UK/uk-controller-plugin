#include "pch/stdafx.h"
#include "components/StandardButtons.h"
#include "graphics/GdiGraphicsInterface.h"


namespace UKControllerPlugin::Components {

    // Coordinates of the close cross
    Gdiplus::Point closeTopLeft = {-5, -5};
    Gdiplus::Point closeBottomRight = {5, 5};
    Gdiplus::Point closeBottomLeft = {-5, 5};
    Gdiplus::Point closeTopRight = {5, -5};

    // Coordinates of the collapse triangle
    Gdiplus::Point collapsePoints[3] = {Gdiplus::Point(0, -5), Gdiplus::Point(5, 5), Gdiplus::Point(-5, 5)};

    // The standard button sizing
    Gdiplus::REAL buttonSize(10);

    /*
     * Draws a standard X-shaped close button of the specified colour.
     */
    std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)> CloseButton(Gdiplus::Color colour)
    {
        auto pen = std::make_shared<Gdiplus::Pen>(colour, 2.0f);
        return [pen](Windows::GdiGraphicsInterface& graphics, const Gdiplus::Rect& drawArea)
        {
            Gdiplus::REAL scaleX = drawArea.Width / buttonSize;
            Gdiplus::REAL scaleY = drawArea.Height / buttonSize;
            ScalePen(pen, scaleX, scaleY);

            graphics.Translated(
                static_cast<Gdiplus::REAL>(5) * scaleX,
                static_cast<Gdiplus::REAL>(5) * scaleY,
                [&graphics, &pen, &drawArea, &scaleX, &scaleY]
                {
                    graphics.Scaled(
                        scaleX,
                        scaleY,
                        [&graphics, &pen]()
                        {
                            graphics.DrawLine(
                                *pen,
                                closeTopLeft,
                                closeBottomRight
                            );
                            graphics.DrawLine(
                                *pen,
                                closeBottomLeft,
                                closeTopRight
                            );
                        }
                    );
                }
            );
        };
    }

    std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)> CloseButton()
    {
        return CloseButton(Gdiplus::Color(227, 227, 227));
    }

    /*
     * Draws a standard triangle shaped collapse button of the specified colour.
     *
     * The state function tells the button whether it is collapsed or not.
     */
    std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)> CollapseButton(
        Gdiplus::Color colour,
        std::function<bool()> stateFunction
    )
    {
        auto brush = std::make_shared<Gdiplus::SolidBrush>(colour);
        return [brush, stateFunction](Windows::GdiGraphicsInterface& graphics, const Gdiplus::Rect& drawArea)
        {
            Gdiplus::REAL scaleX = drawArea.Width / buttonSize;
            Gdiplus::REAL scaleY = drawArea.Height / buttonSize;

            graphics.Translated(
                static_cast<Gdiplus::REAL>(5) * scaleX,
                static_cast<Gdiplus::REAL>(5) * scaleY,
                [&graphics, &stateFunction, &brush, &drawArea, &scaleX, &scaleY]
                {
                    graphics.Rotated(
                        stateFunction() ? static_cast<Gdiplus::REAL>(180) : static_cast<Gdiplus::REAL>(0),
                        [&graphics, &brush, &drawArea, &scaleX, &scaleY]()
                        {
                            graphics.Scaled(
                                scaleX,
                                scaleY,
                                [&graphics, &brush, &drawArea]()
                                {
                                    graphics.FillPolygon(
                                        collapsePoints,
                                        *brush,
                                        3
                                    );
                                }
                            );
                        }
                    );
                }
            );
        };
    }

    std::function<void(Windows::GdiGraphicsInterface& graphics, const Gdiplus::Rect&)> CollapseButton(
        std::function<bool()> stateFunction
    )
    {
        return CollapseButton(Gdiplus::Color(227, 227, 227), stateFunction);
    }

    void ScalePen(std::shared_ptr<Gdiplus::Pen> pen, Gdiplus::REAL& scaleX, Gdiplus::REAL& scaleY)
    {
        pen->ResetTransform();
        pen->ScaleTransform(1 / scaleX, 1 / scaleY);
    }
} // namespace UKControllerPlugin::Components
