#include "SettingRepository.h"
#include "helper/HelperFunctions.h"
#include "windows/WinApiInterface.h"

using UKControllerPlugin::Setting::SettingValue;
using UKControllerPlugin::Windows::WinApiInterface;

namespace UKControllerPlugin::Setting {
    SettingRepository::SettingRepository(WinApiInterface& winApi) : winApi(winApi)
    {
    }

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
            settingsJson = nlohmann::json::parse(this->winApi.ReadFromFile(this->settingFolder + L"/" + widePath));
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
                LogWarning("Duplicate setting for " + it.key() + " in " + relativePath + " has been ignored");
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
        this->UpdateSettingsForFile(this->settings[setting.setting].storagePath);
    }

    /*
        Returns the number of unique settings.
    */
    auto SettingRepository::SettingsCount() const -> size_t
    {
        return this->settings.size();
    }

    /*
        Returns the value for a given setting, or empty string if not found.
    */
    auto SettingRepository::GetSetting(std::string setting, std::string defaultValue) const -> std::string
    {
        return (this->HasSetting(setting)) ? this->settings.at(setting).value : defaultValue;
    }

    /*
        Returns true if a value exists for a given setting.
    */
    auto SettingRepository::HasSetting(std::string setting) const -> bool
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
        this->UpdateSettingsForFile(this->settings[setting].storagePath);
    }

    /*
        Writes all settings to their respective files.
    */
    void SettingRepository::WriteAllSettingsToFile()
    {
        std::map<std::string, std::map<std::string, std::string>> settingsByFile;
        for (const auto& setting : this->settings) {
            settingsByFile[setting.second.storagePath][setting.second.setting] = setting.second.value;
        }

        for (const auto& file : settingsByFile) {
            this->WriteSettingsToFile(file.second, file.first);
        }
    }

    auto SettingRepository::AllSettingsForFile(const std::string& file) -> std::map<std::string, std::string>
    {
        std::map<std::string, std::string> settingsForFile;
        for (const auto& setting : this->settings) {
            if (setting.second.storagePath == file) {
                settingsForFile[setting.second.setting] = setting.second.value;
            }
        }

        return settingsForFile;
    }

    void
    SettingRepository::WriteSettingsToFile(const std::map<std::string, std::string>& settings, const std::string& file)
    {
        try {
            std::wstring widePath = HelperFunctions::ConvertToWideString(file);
            winApi.WriteToFile(this->settingFolder + L"/" + widePath, nlohmann::json(settings).dump(4), true, false);
        } catch (std::ifstream::failure) {
        }
    }

    void SettingRepository::UpdateSettingsForFile(const std::string& file)
    {
        this->WriteSettingsToFile(this->AllSettingsForFile(file), file);
    }
} // namespace UKControllerPlugin::Setting
