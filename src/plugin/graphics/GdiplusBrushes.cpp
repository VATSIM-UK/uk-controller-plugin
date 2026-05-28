#include "GdiplusBrushes.h"
#include "Theme.h"

using UKControllerPlugin::Graphics::Theme;

namespace UKControllerPlugin::Windows {

    GdiplusBrushes::GdiplusBrushes()
    {
        euroscopeBackground = Gdiplus::Color(255, 11, 65, 54);
        green = Gdiplus::Color(255, 0, 255, 0);
        LoadTheme(Graphics::DEFAULT_THEME);
    }

    void GdiplusBrushes::LoadTheme(const Theme& theme)
    {
        backgroundBrush = std::make_shared<Gdiplus::SolidBrush>(theme.background);
        headerBrush = std::make_shared<Gdiplus::SolidBrush>(theme.header);
        highlightedHeaderBrush = std::make_shared<Gdiplus::SolidBrush>(theme.highlightedHeader);
        textBrush = std::make_shared<Gdiplus::SolidBrush>(theme.text);
        highlightedTextBrush = std::make_shared<Gdiplus::SolidBrush>(theme.highlightedText);
        mainAircraftTextBrush = std::make_shared<Gdiplus::SolidBrush>(theme.mainAircraftText);
        highlightedAircraftTextBrush = std::make_shared<Gdiplus::SolidBrush>(theme.highlightedAircraftText);
        timerGreenBrush = std::make_shared<Gdiplus::SolidBrush>(theme.timerGreen);
        timerYellowBrush = std::make_shared<Gdiplus::SolidBrush>(theme.timerYellow);
        timerRedBrush = std::make_shared<Gdiplus::SolidBrush>(theme.timerRed);
        borderBrush = std::make_shared<Gdiplus::SolidBrush>(theme.border);
        borderPen = std::make_shared<Gdiplus::Pen>(theme.border);
        textPen = std::make_shared<Gdiplus::Pen>(theme.text);

        background = theme.background;
        header = theme.header;
        highlightedHeader = theme.highlightedHeader;
        acknowledge = theme.highlightedText;
        border = theme.border;
        text = theme.text;
        mainAircraftText = theme.mainAircraftText;
        highlightedAircraftText = theme.highlightedAircraftText;
        timerGreen = theme.timerGreen;
        timerYellow = theme.timerYellow;
        timerRed = theme.timerRed;

    }

} // namespace UKControllerPlugin::Windows
