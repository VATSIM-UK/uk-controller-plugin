#include "ThemingModule.h"
#include "ColourPaletteDefinitions.h"
#include "bootstrap/PersistenceContainer.h"
#include "euroscope/UserSetting.h"
#include "GdiplusBrushes.h"

using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Windows::GdiplusBrushes;
using UKControllerPlugin::Bootstrap::PersistenceContainer;

namespace UKControllerPlugin::Graphics {
    void ThemingModule::BootstrapPlugin(PersistenceContainer& pc, UserSetting& userSetting)
    {
        std::string palette = userSetting.GetStringEntry("colourPalette", "default");
        ApplyTheme(palette, *pc.brushes);
    }

    void ThemingModule::ApplyTheme(const std::string& paletteName, UKControllerPlugin::Windows::GdiplusBrushes& brushes)
    {
        if (paletteName == "node") {
            brushes.background = ColourPaletteDefinitions::NodeBackground;
            brushes.header = ColourPaletteDefinitions::NodeHeaders;
            brushes.highlightedHeader = ColourPaletteDefinitions::NodeHighlightedHeaders;
            brushes.acknowledge = ColourPaletteDefinitions::NodeHighlightedText;
            brushes.border = ColourPaletteDefinitions::NodeBorder;
            brushes.text = ColourPaletteDefinitions::NodeDefaultText;
            brushes.mainAircraftText = ColourPaletteDefinitions::NodeMainAircraftText;
            brushes.highlightedAircraftText = ColourPaletteDefinitions::NodeHighlightedAircraftText;
            brushes.timerGreen = ColourPaletteDefinitions::NodeTimerGreen;
            brushes.timerYellow = ColourPaletteDefinitions::NodeTimerYellow;
            brushes.timerRed = ColourPaletteDefinitions::NodeTimerRed;
        }
        else if (paletteName == "nerc") {
            brushes.background = ColourPaletteDefinitions::NercBackground;
            brushes.header = ColourPaletteDefinitions::NercHeaders;
            brushes.highlightedHeader = ColourPaletteDefinitions::NercHighlightedHeaders;
            brushes.acknowledge = ColourPaletteDefinitions::NercHighlightedText;
            brushes.border = ColourPaletteDefinitions::NercBorder;
            brushes.text = ColourPaletteDefinitions::NercDefaultText;
            brushes.mainAircraftText = ColourPaletteDefinitions::NercMainAircraftText;
            brushes.highlightedAircraftText = ColourPaletteDefinitions::NercHighlightedAircraftText;
            brushes.timerGreen = ColourPaletteDefinitions::NercTimerGreen;
            brushes.timerYellow = ColourPaletteDefinitions::NercTimerYellow;
            brushes.timerRed = ColourPaletteDefinitions::NercTimerRed;
        }
        else if (paletteName == "nova") {
            brushes.background = ColourPaletteDefinitions::NovaBackground;
            brushes.header = ColourPaletteDefinitions::NovaHeaders;
            brushes.highlightedHeader = ColourPaletteDefinitions::NovaHighlightedHeaders;
            brushes.acknowledge = ColourPaletteDefinitions::NovaHighlightedText;
            brushes.border = ColourPaletteDefinitions::NovaBorder;
            brushes.text = ColourPaletteDefinitions::NovaDefaultText;
            brushes.mainAircraftText = ColourPaletteDefinitions::NovaMainAircraftText;
            brushes.highlightedAircraftText = ColourPaletteDefinitions::NovaHighlightedAircraftText;
            brushes.timerGreen = ColourPaletteDefinitions::NovaTimerGreen;
            brushes.timerYellow = ColourPaletteDefinitions::NovaTimerYellow;
            brushes.timerRed = ColourPaletteDefinitions::NovaTimerRed;
        }
        else if (paletteName == "itec") {
            brushes.background = ColourPaletteDefinitions::ItecBackground;
            brushes.header = ColourPaletteDefinitions::ItecHeaders;
            brushes.highlightedHeader = ColourPaletteDefinitions::ItecHighlightedHeaders;
            brushes.acknowledge = ColourPaletteDefinitions::ItecHighlightedText;
            brushes.border = ColourPaletteDefinitions::ItecBorder;
            brushes.text = ColourPaletteDefinitions::ItecDefaultText;
            brushes.mainAircraftText = ColourPaletteDefinitions::ItecMainAircraftText;
            brushes.highlightedAircraftText = ColourPaletteDefinitions::ItecHighlightedAircraftText;
            brushes.timerGreen = ColourPaletteDefinitions::ItecTimerGreen;
            brushes.timerYellow = ColourPaletteDefinitions::ItecTimerYellow;
            brushes.timerRed = ColourPaletteDefinitions::ItecTimerRed;
        }
        else {
            brushes.background = ColourPaletteDefinitions::DefaultBackground;
            brushes.header = ColourPaletteDefinitions::DefaultHeaders;
            brushes.highlightedHeader = ColourPaletteDefinitions::DefaultHighlightedHeaders;
            brushes.acknowledge = ColourPaletteDefinitions::DefaultHighlightedText;
            brushes.border = ColourPaletteDefinitions::DefaultBorder;
            brushes.text = ColourPaletteDefinitions::DefaultText;
            brushes.mainAircraftText = ColourPaletteDefinitions::DefaultMainAircraftText;
            brushes.highlightedAircraftText = ColourPaletteDefinitions::DefaultHighlightedAircraftText;
            brushes.timerGreen = ColourPaletteDefinitions::DefaultTimerGreen;
            brushes.timerYellow = ColourPaletteDefinitions::DefaultTimerYellow;
            brushes.timerRed = ColourPaletteDefinitions::DefaultTimerRed;
        }
        // Set static/constant brushes
        brushes.white = Gdiplus::Color(255, 255, 255, 255);
        brushes.yellow = Gdiplus::Color(255, 255, 255, 0);
        brushes.euroscopeBackground = Gdiplus::Color(255, 11, 65, 54);
        brushes.black = Gdiplus::Color(255, 0, 0, 0);
        brushes.grey = Gdiplus::Color(255, 125, 125, 125);
        brushes.red = Gdiplus::Color(255, 255, 0, 0);
        brushes.green = Gdiplus::Color(255, 0, 255, 0);
    }
}