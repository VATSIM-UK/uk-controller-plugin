#include "pch/stdafx.h"
#include "components/StandardButtons.h"
#include "graphics/GdiGraphicsInterface.h"


namespace UKControllerPlugin::Components {

    // Coordinates of the close cross
    Gdiplus::Point closeTopLeft = {0, 0};
    Gdiplus::Point closeBottomRight = {10, 10};
    Gdiplus::Point closeBottomLeft = {0, 10};
    Gdiplus::Point closeTopRight = {10, 0};

    // Coordinates of the collapse triangle
    Gdiplus::Point collapsePoints[3] = {Gdiplus::Point(5, 0), Gdiplus::Point(0, 10), Gdiplus::Point(10, 10)};

    // Area of the bounding RECTs
    Gdiplus::Rect boundary = {0, 0, 10, 10};

    std::function<void(Windows::GdiGraphicsInterface& graphics)> CloseButton(Gdiplus::Color colour)
    {
        auto pen = std::make_shared<Gdiplus::Pen>(colour);
        return [pen](Windows::GdiGraphicsInterface& graphics)
        {
            Gdiplus::RectF bounds = graphics.GetClipBounds();
            graphics.Scaled(
                bounds.Width / 10,
                bounds.Height / 10,
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
                    graphics.DrawRect(boundary, *pen);
                }
            );
        };
    }

    std::function<void(Windows::GdiGraphicsInterface& graphics)> CloseButton()
    {
        return CloseButton(Gdiplus::Color(227, 227, 227));
    }

    std::function<void(Windows::GdiGraphicsInterface& graphics)> CollapseButton(
        Gdiplus::Color colour,
        std::function<bool()> stateFunction
    )
    {
        auto brush = std::make_shared<Gdiplus::SolidBrush>(colour);
        auto pen = std::make_shared<Gdiplus::Pen>(colour);
        return [brush, stateFunction, pen](Windows::GdiGraphicsInterface& graphics)
        {
            graphics.Rotated(
                stateFunction() ? static_cast<Gdiplus::REAL>(180) : static_cast<Gdiplus::REAL>(0),
                [&graphics, &brush, &pen]()
                {
                    Gdiplus::RectF bounds = graphics.GetClipBounds();
                    graphics.Scaled(
                        bounds.Width / 10,
                        bounds.Height / 10,
                        [&graphics, &brush, &bounds, &pen]()
                        {
                            graphics.FillPolygon(
                                collapsePoints,
                                *brush,
                                3
                            );
                            graphics.DrawRect(boundary, *pen);
                        }
                    );
                }
            );
        };
    }

    std::function<void(Windows::GdiGraphicsInterface& graphics)> CollapseButton(std::function<bool()> stateFunction)
    {
        return CollapseButton(Gdiplus::Color(227, 227, 227), stateFunction);
    }
} // namespace UKControllerPlugin::Components
