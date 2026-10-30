#pragma once

namespace UKControllerPlugin::Euroscope {
    class UserSetting;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::Theme {

    /*
        Manages the current user settings of the theme module.

        Called from the general settings dialog, and makes changes to the user
        settings as required. However, the actual application of changed
        settings is made by the ThemeUserSettingHandler instance instead.
    */
    class ThemeSettings
    {
        public:
        explicit ThemeSettings(Euroscope::UserSetting& userSettings);

        [[nodiscard]] auto Palette() const -> std::string;

        void SetPalette(std::string id);

        private:
        Euroscope::UserSetting& userSettings;
    };

} // namespace UKControllerPlugin::Theme
