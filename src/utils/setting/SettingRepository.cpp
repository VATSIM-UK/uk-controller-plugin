#include "pch/pch.h"
#include "setting/SettingRepository.h"
#include "windows/WinApiInterface.h"
#include "helper/HelperFunctions.h"

using UKControllerPlugin::Setting::SettingValue;
using UKControllerPlugin::Windows::WinApiInterface;

namespace UKControllerPlugin
{
    namespace Setting
    {
        SettingRepository::SettingRepository(
            WinApiInterface& winApi
        )
            : winApi(winApi)
        { }

        /*
            Read a JSON file and creates settings from it.
        */
        void SettingRepository::AddSettingsFromJsonFile(std::string relativePath, bool overwrite)
        {
            std::wstring widePath = HelperFunctions::ConvertToWideString(relativePath);

            // If the file doesn't exist locally, there's no point.
            if (!this->winApi.FileExists(this->settingFolder + L"/" + widePath)) {
                LogError("Settings file does not exist");
                return;
            }

            nlohmann::json settingsJson;
            try {
                settingsJson = nlohmann::json::parse(
                    this->winApi.ReadFromFile(this->settingFolder + L"/" + widePath)
                );
            } catch (nlohmann::json::exception) {
                LogError("Settings file " + relativePath + " is corrupt");
                return;
            } catch (std::ifstream::failure) {
                LogError("Error reading from file " + relativePath);
                return;
            }

            // Settings should be an object.
            if (!settingsJson.is_object()) {
                LogError("Settings file " + relativePath + " is corrupt");
                return;
            }

            // Process each setting in the file
            for (nlohmann::json::iterator it = settingsJson.begin(); it != settingsJson.end(); ++it) {
                if (this->settings.count(it.key()) > 0 && !overwrite) {
                    LogWarning(
                        "Duplicate setting for " + it.key() + " in " + relativePath + " has been ignored"
                    );
                    continue;
                }

                SettingValue value;
                value.setting = it.key();
                value.value = HelperFunctions::StripQuotesFromJsonString(it->dump());
                value.storagePath = relativePath;

                this->settings[it.key()] = value;
            }
        }

        /*
            Adds a setting to the repo
        */
        void SettingRepository::AddSettingValue(SettingValue setting)
        {
            if (this->HasSetting(setting.setting)) {
                return;
            }

            this->settings[setting.setting] = setting;
        }

        /*
            Returns the number of unique settings.
        */
        size_t SettingRepository::SettingsCount(void) const
        {
            return this->settings.size();
        }

        /*
            Returns the value for a given setting, or empty string if not found.
        */
        std::string SettingRepository::GetSetting(std::string setting, std::string defaultValue) const
        {
            return (this->HasSetting(setting)) ? this->settings.at(setting).value : defaultValue;
        }

        /*
            Returns true if a value exists for a given setting.
        */
        bool SettingRepository::HasSetting(std::string setting) const
        {
            return this->settings.count(setting) > 0;
        }

        /*
            Updates a particular settings value.
        */
        void SettingRepository::UpdateSetting(std::string setting, std::string value)
        {
            if (!this->HasSetting(setting)) {
                return;
            }

            this->settings.at(setting).value = value;
        }

        /*
            Writes all settings to their respective files.
        */
        void SettingRepository::WriteSettingsToFile(void)
        {
            std::map<std::string, std::map<std::string, std::string>> settingsByFile;
            for (
                std::map<std::string, SettingValue>::iterator it = this->settings.begin();
                it != this->settings.end();
                ++it
            ) {
                settingsByFile[it->second.storagePath][it->second.setting] = it->second.value;
            }

            for (
                std::map<std::string, std::map<std::string, std::string>>::iterator it = settingsByFile.begin();
                it != settingsByFile.end();
                ++it
            ) {
                try {
                    std::wstring widePath = HelperFunctions::ConvertToWideString(it->first);
                    winApi.WriteToFile(
                        this->settingFolder + L"/" + widePath,
                        nlohmann::json(it->second).dump(4),
                        true,
                        false
                    );
                } catch (std::ifstream::failure) { }
            }
        }
    } // namespace Setting
} // namespace UKControllerPlugin
