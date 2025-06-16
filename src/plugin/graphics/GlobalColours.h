#pragma once
#include "GdiGraphicsInterface.h"

namespace UKControllerPlugin {
    namespace Euroscope {
        class UserSetting;
    } // namespace Euroscope
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Graphics {

        // Default colour palette name
        const std::string DEFAULT_COLOUR_PALETTE = "default";

        // Declare global colour variables
        extern Gdiplus::Color Background;
        extern Gdiplus::Color Border;
        extern Gdiplus::Color Headers;
        extern Gdiplus::Color HighlightedHeaders;
        extern Gdiplus::Color DefaultText;
        extern Gdiplus::Color HighlightedText;
        extern Gdiplus::Color MainAircraftText;
        extern Gdiplus::Color HighlightedAircraftText;
        extern Gdiplus::Color TimerGreen;
        extern Gdiplus::Color TimerYellow;
        extern Gdiplus::Color TimerRed;

        class GlobalColours
        {
            public:
            static void SetGlobalColours(const std::string& paletteName);
            static void LoadDefaultUserSettings(UKControllerPlugin::Euroscope::UserSetting& userSetting);
        };
}