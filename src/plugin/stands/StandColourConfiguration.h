#pragma once
#include <map>
#include <string>
#include <string_view>
#include <Windows.h>

namespace UKControllerPlugin::Euroscope {
    class UserSetting;
}

namespace UKControllerPlugin::Stands {
    /*
        Manages colour configuration for stand assignments based on their source.
        Colours are persisted to EuroScope UserSettings and can be customized without recompilation.
    */
    class StandColourConfiguration
    {
        public:
        explicit StandColourConfiguration(UKControllerPlugin::Euroscope::UserSetting& userSetting);
        explicit StandColourConfiguration();

        /*
            Get the colour for a given assignment source.
            Returns a default grey if source is not found or not set.
        */
        [[nodiscard]] auto GetColourForSource(const std::string& source) const -> COLORREF;

        /*
            Load colours from UserSettings, with fallback to defaults.
        */
        void LoadFromUserSettings();

        private:
        // Reference to user settings for persistence (null if not available)
        UKControllerPlugin::Euroscope::UserSetting* userSetting;

        // Map of source to RGB colour
        std::map<std::string, COLORREF, std::less<>> sourceColours;

        // Default colour (grey) for unknown sources
        static constexpr COLORREF DEFAULT_COLOUR = RGB(180, 180, 180);

        // Default colours if not configured
        static constexpr COLORREF DEFAULT_USER_COLOUR = RGB(255, 255, 255);      // white
        static constexpr COLORREF DEFAULT_RESERVATION_COLOUR = RGB(255, 255, 0); // yellow
        static constexpr COLORREF DEFAULT_VAA_COLOUR = RGB(0, 255, 255);         // cyan
        static constexpr COLORREF DEFAULT_SYSTEM_COLOUR = RGB(180, 180, 180);    // grey

        // UserSetting key prefix for colours
        static constexpr std::string_view SETTING_PREFIX = "stand_colour_";
    };
} // namespace UKControllerPlugin::Stands
