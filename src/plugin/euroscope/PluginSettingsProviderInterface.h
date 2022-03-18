#pragma once

namespace UKControllerPlugin::Euroscope {
    class UserSetting;

    class PluginSettingsProviderInterface
    {
        public:
        virtual ~PluginSettingsProviderInterface() = default;

        /**
         * Load any settings that we care about from the settings file.
         */
        virtual void LoadSettings(Euroscope::UserSetting& settings) = 0;

        /**
         * Save our settings to the settings file.
         */
        virtual void SaveSettings(Euroscope::UserSetting& settings) = 0;
    };
} // namespace UKControllerPlugin::Euroscope
