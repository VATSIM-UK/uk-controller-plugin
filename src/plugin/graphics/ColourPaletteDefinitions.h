#pragma once
#include "GdiGraphicsInterface.h"

namespace UKControllerPlugin {
    namespace Graphics {
        /*
            GDIPlus Definitions for the various colour palettes.
        */
        struct ColourPaletteDefinitions
        {
            // DEFAULT
            static const Gdiplus::Color DefaultBackground;
            static const Gdiplus::Color DefaultBorder;
            static const Gdiplus::Color DefaultHeaders;
            static const Gdiplus::Color DefaultHighlightedHeaders;
            static const Gdiplus::Color DefaultText;
            static const Gdiplus::Color DefaultHighlightedText;
            static const Gdiplus::Color DefaultMainAircraftText;
            static const Gdiplus::Color DefaultHighlightedAircraftText;
            static const Gdiplus::Color DefaultTimerGreen;
            static const Gdiplus::Color DefaultTimerYellow;
            static const Gdiplus::Color DefaultTimerRed;

            // NODE
            static const Gdiplus::Color NodeBackground;              // #000000
            static const Gdiplus::Color NodeBorder;                  // #ffffff
            static const Gdiplus::Color NodeHeaders;                 // #a06f70
            static const Gdiplus::Color NodeHighlightedHeaders;      // #ff6432
            static const Gdiplus::Color NodeDefaultText;             // #ffffff
            static const Gdiplus::Color NodeHighlightedText;         // #e8a300
            static const Gdiplus::Color NodeMainAircraftText;        // #68dd45
            static const Gdiplus::Color NodeHighlightedAircraftText; // #e8a300
            static const Gdiplus::Color NodeTimerGreen;              // #68dd45
            static const Gdiplus::Color NodeTimerYellow;             // #e8a300
            static const Gdiplus::Color NodeTimerRed;                // #ff6432

            // NERC
            static const Gdiplus::Color NercBackground;              // #969696
            static const Gdiplus::Color NercBorder;                  // #535353
            static const Gdiplus::Color NercHeaders;                 // #878787
            static const Gdiplus::Color NercHighlightedHeaders;      // #ffffff
            static const Gdiplus::Color NercDefaultText;             // #000000
            static const Gdiplus::Color NercHighlightedText;         // #ffffff
            static const Gdiplus::Color NercMainAircraftText;        // #000000
            static const Gdiplus::Color NercHighlightedAircraftText; // #ffffff
            static const Gdiplus::Color NercTimerGreen;              // #B4FF7D
            static const Gdiplus::Color NercTimerYellow;             // #FFD800
            static const Gdiplus::Color NercTimerRed;                // #C00000

            // NOVA
            static const Gdiplus::Color NovaBackground;              // #B4B4B4
            static const Gdiplus::Color NovaBorder;                  // #535353
            static const Gdiplus::Color NovaHeaders;                 // #008CC8
            static const Gdiplus::Color NovaHighlightedHeaders;      // #ffffff
            static const Gdiplus::Color NovaDefaultText;             // #000000
            static const Gdiplus::Color NovaHighlightedText;         // #ffffff
            static const Gdiplus::Color NovaMainAircraftText;        // #000000
            static const Gdiplus::Color NovaHighlightedAircraftText; // #ffffff
            static const Gdiplus::Color NovaTimerGreen;              // #ffffff
            static const Gdiplus::Color NovaTimerYellow;             // #FFD800
            static const Gdiplus::Color NovaTimerRed;                // #C00000

            // iTEC
            static const Gdiplus::Color ItecBackground;              // #C3C3B9
            static const Gdiplus::Color ItecBorder;                  // #000000
            static const Gdiplus::Color ItecHeaders;                 // #D6D896
            static const Gdiplus::Color ItecHighlightedHeaders;      // #ffffff
            static const Gdiplus::Color ItecDefaultText;             // #000000
            static const Gdiplus::Color ItecHighlightedText;         // #0000FF
            static const Gdiplus::Color ItecMainAircraftText;        // #000000
            static const Gdiplus::Color ItecHighlightedAircraftText; // #0000FF
            static const Gdiplus::Color ItecTimerGreen;              // #0000FF
            static const Gdiplus::Color ItecTimerYellow;             // #826F00
            static const Gdiplus::Color ItecTimerRed;                // #C00000
        };
    } // namespace Graphics
} // namespace UKControllerPlugin