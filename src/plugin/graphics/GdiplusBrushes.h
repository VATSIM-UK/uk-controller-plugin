#pragma once

namespace UKControllerPlugin {
    namespace Windows {

        struct GdiplusBrushes
        {
            Gdiplus::Color white;
            Gdiplus::Color yellow;
            Gdiplus::Color euroscopeBackground;
            Gdiplus::Color black;
            Gdiplus::Color grey;
            Gdiplus::Color red;
            Gdiplus::Color green;

            // Add themed colors
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

    } // namespace Windows
} // namespace UKControllerPlugin
