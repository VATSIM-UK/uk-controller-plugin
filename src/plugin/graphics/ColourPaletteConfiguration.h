#pragma once
#include "GdiGraphicsInterface.h"
#include "euroscope/UserSetting.h"

using UKControllerPlugin::Euroscope::UserSetting;

namespace UKControllerPlugin {
    namespace Graphics {

        // Global definitions for the current colour palette.
        struct ColourPaletteConfiguration {
            Gdiplus::Color Background;
            Gdiplus::Color Border;
            Gdiplus::Color Headers;
            Gdiplus::Color HighlightedHeaders;
            Gdiplus::Color DefaultText;
            Gdiplus::Color HighlightedText;
            Gdiplus::Color MainAircraftText;
            Gdiplus::Color HighlightedAircraftText;
            Gdiplus::Color TimerGreen;
            Gdiplus::Color TimerYellow;
            Gdiplus::Color TimerRed;
        };

        const std::map<std::string, std::wstring> colourPaletteMap{
            {"default", L"Default"},
            {"node", L"NODE"},
            {"nerc", L"NERC"},
            {"nova", L"NOVA"},
            {"itec", L"iTEC"},
        };

        const std::string DEFAULT_COLOUR_PALETTE = "default";

        std::string GetSelectedColourPalette(UserSetting& userSetting);
    }
}