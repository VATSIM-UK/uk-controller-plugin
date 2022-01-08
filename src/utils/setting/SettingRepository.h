#pragma once
#include "setting/SettingValue.h"

namespace UKControllerPlugin::Windows {
    class WinApiInterface;
} // namespace UKControllerPlugin::Windows

namespace UKControllerPlugin::Setting {

    /*
        A class for loading and storing settings from file. This class is used primarily
        to store global settings for the plugin that are not specific to a particular EuroScope
        profile or ASR.

        Settings must have a unique key, no matter which file they've come from.
    */
    class SettingRepository
    {
        public:
        explicit SettingRepository(UKControllerPlugin::Windows::WinApiInterface& winApi);
        void AddSettingsFromJsonFile(std::string relativePath, bool overwrite = false);
        void AddSettingValue(UKControllerPlugin::Setting::SettingValue setting);
        [[nodiscard]] auto SettingsCount() const -> size_t;
        [[nodiscard]] auto GetSetting(std::string setting, std::string defaultValue = "") const -> std::string;
        [[nodiscard]] auto HasSetting(std::string setting) const -> bool;
        void UpdateSetting(std::string setting, std::string value);
        void WriteAllSettingsToFile();

        // The folder where we put all our settings.
        const std::wstring settingFolder = L"settings";

        private:
        void UpdateSettingsForFile(const std::string& file);
        [[nodiscard]] auto AllSettingsForFile(const std::string& file) -> std::map<std::string, std::string>;
        void WriteSettingsToFile(const std::map<std::string, std::string>& settings, const std::string& file);

        // Settings
        std::map<std::string, UKControllerPlugin::Setting::SettingValue> settings;

        // Interface to windows
        UKControllerPlugin::Windows::WinApiInterface& winApi;
    };
} // namespace UKControllerPlugin::Setting
