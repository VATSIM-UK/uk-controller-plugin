#include "GlobalColours.h"
#include "ColourPaletteDefinitions.h"
#include "euroscope/UserSettingAwareCollection.h"
#include "euroscope/UserSetting.h"
#include <sstream>
#include "euroscope/GeneralSettingsEntries.h"

using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Euroscope::UserSettingAwareCollection;
using UKControllerPlugin::Euroscope::GeneralSettingsEntries;

namespace UKControllerPlugin::Graphics {
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


        std::string ColorToString(const Gdiplus::Color& color) {
            std::ostringstream oss;
            oss << "ARGB("
                << static_cast<int>(color.GetA()) << ","
                << static_cast<int>(color.GetR()) << ","
                << static_cast<int>(color.GetG()) << ","
                << static_cast<int>(color.GetB()) << ")";
            return oss.str();
        }

        void GlobalColours::LoadDefaultUserSettings(UserSetting& userSetting)
        {
            LogInfo("Loading default user settings for global colours");
            // Set default colours if not already set
            if (!userSetting.HasEntry(GeneralSettingsEntries::colourPaletteSettingsKey)) {
                userSetting.Save(GeneralSettingsEntries::colourPaletteSettingsKey,
                    GeneralSettingsEntries::colourPaletteSettingsDescription,
                    DEFAULT_COLOUR_PALETTE);
            }
            GlobalColours::SetGlobalColours(userSetting.GetStringEntry("colourPalette", "default"));
        }

        void GlobalColours::SetGlobalColours(const std::string& paletteName) {
            LogInfo("Setting global colours to palette: " + paletteName);

            auto logColour = [](const std::string& name, const Gdiplus::Color& color) {
                LogInfo(name + " set to " + ColorToString(color));
            };

            if (paletteName == "default") {
                Background = ColourPaletteDefinitions::NodeBackground;
                logColour("Background", Background);
                Border = ColourPaletteDefinitions::NodeBorder;
                logColour("Border", Border);
                Headers = ColourPaletteDefinitions::NodeHeaders;
                logColour("Headers", Headers);
                HighlightedHeaders = ColourPaletteDefinitions::NodeHighlightedHeaders;
                logColour("HighlightedHeaders", HighlightedHeaders);
                DefaultText = ColourPaletteDefinitions::NodeDefaultText;
                logColour("DefaultText", DefaultText);
                HighlightedText = ColourPaletteDefinitions::NodeHighlightedText;
                logColour("HighlightedText", HighlightedText);
                MainAircraftText = ColourPaletteDefinitions::NodeMainAircraftText;
                logColour("MainAircraftText", MainAircraftText);
                HighlightedAircraftText = ColourPaletteDefinitions::NodeHighlightedAircraftText;
                logColour("HighlightedAircraftText", HighlightedAircraftText);
                TimerGreen = ColourPaletteDefinitions::NodeTimerGreen;
                logColour("TimerGreen", TimerGreen);
                TimerYellow = ColourPaletteDefinitions::NodeTimerYellow;
                logColour("TimerYellow", TimerYellow);
                TimerRed = ColourPaletteDefinitions::NodeTimerRed;
                logColour("TimerRed", TimerRed);
            }
            else if (paletteName == "nerc") {
                Background = ColourPaletteDefinitions::NercBackground;
                logColour("Background", Background);
                Border = ColourPaletteDefinitions::NercBorder;
                logColour("Border", Border);
                Headers = ColourPaletteDefinitions::NercHeaders;
                logColour("Headers", Headers);
                HighlightedHeaders = ColourPaletteDefinitions::NercHighlightedHeaders;
                logColour("HighlightedHeaders", HighlightedHeaders);
                DefaultText = ColourPaletteDefinitions::NercDefaultText;
                logColour("DefaultText", DefaultText);
                HighlightedText = ColourPaletteDefinitions::NercHighlightedText;
                logColour("HighlightedText", HighlightedText);
                MainAircraftText = ColourPaletteDefinitions::NercMainAircraftText;
                logColour("MainAircraftText", MainAircraftText);
                HighlightedAircraftText = ColourPaletteDefinitions::NercHighlightedAircraftText;
                logColour("HighlightedAircraftText", HighlightedAircraftText);
                TimerGreen = ColourPaletteDefinitions::NercTimerGreen;
                logColour("TimerGreen", TimerGreen);
                TimerYellow = ColourPaletteDefinitions::NercTimerYellow;
                logColour("TimerYellow", TimerYellow);
                TimerRed = ColourPaletteDefinitions::NercTimerRed;
                logColour("TimerRed", TimerRed);
            }
            else if (paletteName == "nova") {
                Background = ColourPaletteDefinitions::NovaBackground;
                logColour("Background", Background);
                Border = ColourPaletteDefinitions::NovaBorder;
                logColour("Border", Border);
                Headers = ColourPaletteDefinitions::NovaHeaders;
                logColour("Headers", Headers);
                HighlightedHeaders = ColourPaletteDefinitions::NovaHighlightedHeaders;
                logColour("HighlightedHeaders", HighlightedHeaders);
                DefaultText = ColourPaletteDefinitions::NovaDefaultText;
                logColour("DefaultText", DefaultText);
                HighlightedText = ColourPaletteDefinitions::NovaHighlightedText;
                logColour("HighlightedText", HighlightedText);
                MainAircraftText = ColourPaletteDefinitions::NovaMainAircraftText;
                logColour("MainAircraftText", MainAircraftText);
                HighlightedAircraftText = ColourPaletteDefinitions::NovaHighlightedAircraftText;
                logColour("HighlightedAircraftText", HighlightedAircraftText);
                TimerGreen = ColourPaletteDefinitions::NovaTimerGreen;
                logColour("TimerGreen", TimerGreen);
                TimerYellow = ColourPaletteDefinitions::NovaTimerYellow;
                logColour("TimerYellow", TimerYellow);
                TimerRed = ColourPaletteDefinitions::NovaTimerRed;
                logColour("TimerRed", TimerRed);
            }
            else if (paletteName == "itec") {
                Background = ColourPaletteDefinitions::ItecBackground;
                logColour("Background", Background);
                Border = ColourPaletteDefinitions::ItecBorder;
                logColour("Border", Border);
                Headers = ColourPaletteDefinitions::ItecHeaders;
                logColour("Headers", Headers);
                HighlightedHeaders = ColourPaletteDefinitions::ItecHighlightedHeaders;
                logColour("HighlightedHeaders", HighlightedHeaders);
                DefaultText = ColourPaletteDefinitions::ItecDefaultText;
                logColour("DefaultText", DefaultText);
                HighlightedText = ColourPaletteDefinitions::ItecHighlightedText;
                logColour("HighlightedText", HighlightedText);
                MainAircraftText = ColourPaletteDefinitions::ItecMainAircraftText;
                logColour("MainAircraftText", MainAircraftText);
                HighlightedAircraftText = ColourPaletteDefinitions::ItecHighlightedAircraftText;
                logColour("HighlightedAircraftText", HighlightedAircraftText);
                TimerGreen = ColourPaletteDefinitions::ItecTimerGreen;
                logColour("TimerGreen", TimerGreen);
                TimerYellow = ColourPaletteDefinitions::ItecTimerYellow;
                logColour("TimerYellow", TimerYellow);
                TimerRed = ColourPaletteDefinitions::ItecTimerRed;
                logColour("TimerRed", TimerRed);
            }
            else {
                Background = ColourPaletteDefinitions::DefaultBackground;
                logColour("Background", Background);
                Border = ColourPaletteDefinitions::DefaultBorder;
                logColour("Border", Border);
                Headers = ColourPaletteDefinitions::DefaultHeaders;
                logColour("Headers", Headers);
                HighlightedHeaders = ColourPaletteDefinitions::DefaultHighlightedHeaders;
                logColour("HighlightedHeaders", HighlightedHeaders);
                DefaultText = ColourPaletteDefinitions::DefaultText;
                logColour("DefaultText", DefaultText);
                HighlightedText = ColourPaletteDefinitions::DefaultHighlightedText;
                logColour("HighlightedText", HighlightedText);
                MainAircraftText = ColourPaletteDefinitions::DefaultMainAircraftText;
                logColour("MainAircraftText", MainAircraftText);
                HighlightedAircraftText = ColourPaletteDefinitions::DefaultHighlightedAircraftText;
                logColour("HighlightedAircraftText", HighlightedAircraftText);
                TimerGreen = ColourPaletteDefinitions::DefaultTimerGreen;
                logColour("TimerGreen", TimerGreen);
                TimerYellow = ColourPaletteDefinitions::DefaultTimerYellow;
                logColour("TimerYellow", TimerYellow);
                TimerRed = ColourPaletteDefinitions::DefaultTimerRed;
                logColour("TimerRed", TimerRed);
            }
        }
}