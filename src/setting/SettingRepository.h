#pragma once
#include "setting/SettingValue.h"

namespace UKControllerPlugin {
    namespace Windows {
        class WinApiInterface;
    }  // namespace Windows
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Setting {

        /*
            A class for loading and storing settings from file. This class is used primarily
            to store global settings for the plugin that are not specific to a particular EuroScope
            profile or ASR.

            Settings must have a unique key, no matter which file they've come from.
        */
        class SettingRepository
        {
            public:
                explicit SettingRepository(UKControllerPlugin::Windows::WinApiInterface & winApi);
                void AddSettingsFromJsonFile(std::string relativePath);
                void AddSettingValue(UKControllerPlugin::Setting::SettingValue setting);
                size_t SettingsCount(void) const;
                std::string GetSetting(std::string setting) const;
                bool HasSetting(std::string setting) const;
                void UpdateSetting(std::string setting, std::string value);
                void WriteSettingsToFile(void);

                // The folder where we put all our settings.
                const std::string settingFolder = "settings";

            private:

                // Settings
                std::map<std::string, UKControllerPlugin::Setting::SettingValue> settings;

                // Interface to windows
                UKControllerPlugin::Windows::WinApiInterface & winApi;
        };
    }  // namespace Setting
}  // namespace UKControllerPlugin
