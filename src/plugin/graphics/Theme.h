#pragma once

namespace UKControllerPlugin::Graphics {

    /*
        used to populate GdiplusBrushes via LoadTheme()
    */
    struct Theme
    {
        Gdiplus::Color background;
        Gdiplus::Color border;
        Gdiplus::Color header;
        Gdiplus::Color highlightedHeader;
        Gdiplus::Color text;
        Gdiplus::Color highlightedText;
        Gdiplus::Color mainAircraftText;
        Gdiplus::Color highlightedAircraftText;
        Gdiplus::Color timerGreen;
        Gdiplus::Color timerYellow;
        Gdiplus::Color timerRed;
    };

    extern const Theme DEFAULT_THEME;
    extern const Theme NODE_THEME;
    extern const Theme NERC_THEME;
    extern const Theme NOVA_THEME;
    extern const Theme ITEC_THEME;

} // namespace UKControllerPlugin::Graphics
