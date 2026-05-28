#pragma once

#include <memory>

namespace UKControllerPlugin::Graphics {
    struct Theme;
} // namespace UKControllerPlugin::Graphics

namespace UKControllerPlugin::Windows {

    /*
        Stores all GDI+ brush and pen objects used for rendering.

        Consumers use the shared_ptr members directly
    */
    struct GdiplusBrushes
    {
        // themed brushes - reconstructed by LoadTheme
        std::shared_ptr<Gdiplus::SolidBrush> backgroundBrush;
        std::shared_ptr<Gdiplus::SolidBrush> headerBrush;
        std::shared_ptr<Gdiplus::SolidBrush> highlightedHeaderBrush;
        std::shared_ptr<Gdiplus::SolidBrush> textBrush;
        std::shared_ptr<Gdiplus::SolidBrush> borderBrush;
        std::shared_ptr<Gdiplus::SolidBrush> highlightedTextBrush;
        std::shared_ptr<Gdiplus::SolidBrush> mainAircraftTextBrush;
        std::shared_ptr<Gdiplus::SolidBrush> highlightedAircraftTextBrush;
        std::shared_ptr<Gdiplus::SolidBrush> timerGreenBrush;
        std::shared_ptr<Gdiplus::SolidBrush> timerYellowBrush;
        std::shared_ptr<Gdiplus::SolidBrush> timerRedBrush;
        std::shared_ptr<Gdiplus::Pen> borderPen;
        std::shared_ptr<Gdiplus::Pen> textPen;

        // static colours (not themeable)
        Gdiplus::Color euroscopeBackground;
        Gdiplus::Color green;

        GdiplusBrushes();

        void LoadTheme(const Graphics::Theme& theme);

        Gdiplus::Color background;
        Gdiplus::Color header;
        Gdiplus::Color highlightedHeader;
        Gdiplus::Color acknowledge;
        Gdiplus::Color border;
        Gdiplus::Color text;
        Gdiplus::Color mainAircraftText;
        Gdiplus::Color highlightedAircraftText;
        Gdiplus::Color timerGreen;
        Gdiplus::Color timerYellow;
        Gdiplus::Color timerRed;
    };

} // namespace UKControllerPlugin::Windows
