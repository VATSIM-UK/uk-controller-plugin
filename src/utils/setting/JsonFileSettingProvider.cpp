#include "JsonFileSettingProvider.h"
#include "helper/HelperFunctions.h"
#include "windows/WinApiInterface.h"

namespace UKControllerPlugin::Setting {

    JsonFileSettingProvider::JsonFileSettingProvider(
        const std::wstring filename, std::set<std::string> providedSettings, Windows::WinApiInterface& windows)
        : filename(std::move(filename)), providedSettings(std::move(providedSettings)), windows(windows),
          loadedSettings(LoadFromFile())
    {
    }

    auto JsonFileSettingProvider::Get(const std::string& key) -> std::string
    {
        if (loadedSettings.count(key) == 0) {
            return "";
        }

        return loadedSettings.at(key);
    }

    void JsonFileSettingProvider::Save(const std::string& key, const std::string& value)
    {
        loadedSettings[key] = value;
        windows.WriteToFile(L"settings/" + filename, nlohmann::json(loadedSettings).dump(), true, false);
    }

    auto JsonFileSettingProvider::Provides() -> const std::set<std::string>&
    {
        return providedSettings;
    }

    auto JsonFileSettingProvider::LoadFromFile() const -> std::map<std::string, std::string>
    {
        nlohmann::json data;
        try {
            data = nlohmann::json::parse(windows.ReadFromFile(L"settings/" + filename));
        } catch (nlohmann::json::exception&) {
            LogError("Invalid JSON in setting file " + HelperFunctions::ConvertToRegularString(filename));
            return {};
        }

        if (!data.is_object()) {
            LogError("JSON not object in setting file " + HelperFunctions::ConvertToRegularString(filename));
            return {};
        }

        for (auto it = data.cbegin(); it != data.cend(); ++it) {
            if (!it->is_string()) {
                LogError(
                    "JSON value not a string in setting file " + HelperFunctions::ConvertToRegularString(filename));
                return {};
            }
        }

        return data.get<std::map<std::string, std::string>>();
    }

    void JsonFileSettingProvider::Reload()
    {
        this->loadedSettings = this->LoadFromFile();
    }
} // namespace UKControllerPlugin::Setting
