#include "ColourPaletteDefinitions.h"
#include "GdiGraphicsInterface.h"
#include "ColourPaletteConfiguration.h"
#include "euroscope/GeneralSettingsEntries.h"
#include "euroscope/UserSetting.h"

using UKControllerPlugin::Graphics::ColourPaletteDefinitions;
using UKControllerPlugin::Euroscope::GeneralSettingsEntries;
using UKControllerPlugin::Euroscope::UserSetting;

namespace UKControllerPlugin {
    namespace Graphics {
        /*
            Set the colour palette definitions based on the selected palette name.
        */
        ColourPaletteConfiguration SetColourPalette(UserSetting& userSetting) {
            ColourPaletteConfiguration palette;
            std::string selectedPaletteName = userSetting.GetStringEntry("colour_palette", DEFAULT_COLOUR_PALETTE);

            if (selectedPaletteName == "node") {
                palette.Background = Gdiplus::Color(ColourPaletteDefinitions::NodeBackground);
                palette.Border = Gdiplus::Color(ColourPaletteDefinitions::NodeBorder);
                palette.Headers = Gdiplus::Color(ColourPaletteDefinitions::NodeHeaders);
                palette.HighlightedHeaders = Gdiplus::Color(ColourPaletteDefinitions::NodeHighlightedHeaders);
                palette.DefaultText = Gdiplus::Color(ColourPaletteDefinitions::NodeDefaultText);
                palette.HighlightedText = Gdiplus::Color(ColourPaletteDefinitions::NodeHighlightedText);
                palette.MainAircraftText = Gdiplus::Color(ColourPaletteDefinitions::NodeMainAircraftText);
                palette.HighlightedAircraftText = Gdiplus::Color(ColourPaletteDefinitions::NodeHighlightedAircraftText);
                palette.TimerGreen = Gdiplus::Color(ColourPaletteDefinitions::NodeTimerGreen);
                palette.TimerYellow = Gdiplus::Color(ColourPaletteDefinitions::NodeTimerYellow);
                palette.TimerRed = Gdiplus::Color(ColourPaletteDefinitions::NodeTimerRed);
            } else if (selectedPaletteName == "nerc") {
                palette.Background = Gdiplus::Color(ColourPaletteDefinitions::NercBackground);
                palette.Border = Gdiplus::Color(ColourPaletteDefinitions::NercBorder);
                palette.Headers = Gdiplus::Color(ColourPaletteDefinitions::NercHeaders);
                palette.HighlightedHeaders = Gdiplus::Color(ColourPaletteDefinitions::NercHighlightedHeaders);
                palette.DefaultText = Gdiplus::Color(ColourPaletteDefinitions::NercDefaultText);
                palette.HighlightedText = Gdiplus::Color(ColourPaletteDefinitions::NercHighlightedText);
                palette.MainAircraftText = Gdiplus::Color(ColourPaletteDefinitions::NercMainAircraftText);
                palette.HighlightedAircraftText = Gdiplus::Color(ColourPaletteDefinitions::NercHighlightedAircraftText);
                palette.TimerGreen = Gdiplus::Color(ColourPaletteDefinitions::NercTimerGreen);
                palette.TimerYellow = Gdiplus::Color(ColourPaletteDefinitions::NercTimerYellow);
                palette.TimerRed = Gdiplus::Color(ColourPaletteDefinitions::NercTimerRed);
            } else if (selectedPaletteName == "nova") {
                palette.Background = Gdiplus::Color(ColourPaletteDefinitions::NovaBackground);
                palette.Border = Gdiplus::Color(ColourPaletteDefinitions::NovaBorder);
                palette.Headers = Gdiplus::Color(ColourPaletteDefinitions::NovaHeaders);
                palette.HighlightedHeaders = Gdiplus::Color(ColourPaletteDefinitions::NovaHighlightedHeaders);
                palette.DefaultText = Gdiplus::Color(ColourPaletteDefinitions::NovaDefaultText);
                palette.HighlightedText = Gdiplus::Color(ColourPaletteDefinitions::NovaHighlightedText);
                palette.MainAircraftText = Gdiplus::Color(ColourPaletteDefinitions::NovaMainAircraftText);
                palette.HighlightedAircraftText = Gdiplus::Color(ColourPaletteDefinitions::NovaHighlightedAircraftText);
                palette.TimerGreen = Gdiplus::Color(ColourPaletteDefinitions::NovaTimerGreen);
                palette.TimerYellow = Gdiplus::Color(ColourPaletteDefinitions::NovaTimerYellow);
                palette.TimerRed = Gdiplus::Color(ColourPaletteDefinitions::NovaTimerRed);
            } else if (selectedPaletteName == "itec") {
                palette.Background = Gdiplus::Color(ColourPaletteDefinitions::ItecBackground);
                palette.Border = Gdiplus::Color(ColourPaletteDefinitions::ItecBorder);
                palette.Headers = Gdiplus::Color(ColourPaletteDefinitions::ItecHeaders);
                palette.HighlightedHeaders = Gdiplus::Color(ColourPaletteDefinitions::ItecHighlightedHeaders);
                palette.DefaultText = Gdiplus::Color(ColourPaletteDefinitions::ItecDefaultText);
                palette.HighlightedText = Gdiplus::Color(ColourPaletteDefinitions::ItecHighlightedText);
                palette.MainAircraftText = Gdiplus::Color(ColourPaletteDefinitions::ItecMainAircraftText);
                palette.HighlightedAircraftText = Gdiplus::Color(ColourPaletteDefinitions::ItecHighlightedAircraftText);
                palette.TimerGreen = Gdiplus::Color(ColourPaletteDefinitions::ItecTimerGreen);
                palette.TimerYellow = Gdiplus::Color(ColourPaletteDefinitions::ItecTimerYellow);
                palette.TimerRed = Gdiplus::Color(ColourPaletteDefinitions::ItecTimerRed);
            } 
            else {
                // Default to the default palette WIP
                palette.Background = Gdiplus::Color(ColourPaletteDefinitions::NodeBackground);
                palette.Border = Gdiplus::Color(ColourPaletteDefinitions::NodeBorder);
                palette.Headers = Gdiplus::Color(ColourPaletteDefinitions::NodeHeaders);
                palette.HighlightedHeaders = Gdiplus::Color(ColourPaletteDefinitions::NodeHighlightedHeaders);
                palette.DefaultText = Gdiplus::Color(ColourPaletteDefinitions::NodeDefaultText);
                palette.HighlightedText = Gdiplus::Color(ColourPaletteDefinitions::NodeHighlightedText);
                palette.MainAircraftText = Gdiplus::Color(ColourPaletteDefinitions::NodeMainAircraftText);
                palette.HighlightedAircraftText = Gdiplus::Color(ColourPaletteDefinitions::NodeHighlightedAircraftText);
                palette.TimerGreen = Gdiplus::Color(ColourPaletteDefinitions::NodeTimerGreen);
                palette.TimerYellow = Gdiplus::Color(ColourPaletteDefinitions::NodeTimerYellow);
                palette.TimerRed = Gdiplus::Color(ColourPaletteDefinitions::NodeTimerRed);
            }
            return palette;
        }
    }
}