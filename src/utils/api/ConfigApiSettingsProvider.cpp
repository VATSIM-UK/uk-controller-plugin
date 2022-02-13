#include "ApiSettings.h"
#include "ConfigApiSettingsProvider.h"
#include "setting/SettingRepositoryInterface.h"
#include "windows/WinApiInterface.h"

using UKControllerPlugin::Setting::SettingRepositoryInterface;
using UKControllerPlugin::Windows::WinApiInterface;

namespace UKControllerPluginUtils::Api {

    ConfigApiSettingsProvider::ConfigApiSettingsProvider(
        SettingRepositoryInterface& settingRepository, WinApiInterface& windows)
        : settingRepository(settingRepository), windows(windows)
    {
    }

    ConfigApiSettingsProvider::~ConfigApiSettingsProvider() = default;

    auto ConfigApiSettingsProvider::Get() -> ApiSettings&
    {
        if (!this->settings) {
            this->settings = std::make_unique<ApiSettings>(
                settingRepository.GetSetting(API_URL_SETTING, DEFAULT_API_URL),
                settingRepository.GetSetting(API_KEY_SETTING, DEFAULT_API_KEY));
        }

        return *this->settings;
    }

    auto ConfigApiSettingsProvider::Reload() -> bool
    {
        // Select the file to get settings from
        COMDLG_FILTERSPEC fileTypes[] = {
            {L"JSON", L"*.json"},
        };

        std::wstring filePath = windows.FileOpenDialog(L"Select API Settings File", 1, fileTypes);
        if (filePath == L"") {
            LogInfo("User did not select a valid key file to replacte");
            return false;
        }

        // Write file
        windows.WriteToFile(L"settings/api-settings.json", windows.ReadFromFile(filePath, false), true, false);

        // Trigger a reload
        this->settingRepository.ReloadSetting(API_KEY_SETTING);
        this->settingRepository.ReloadSetting(API_URL_SETTING);

        this->settings->Url(settingRepository.GetSetting(API_URL_SETTING, DEFAULT_API_URL));
        this->settings->Key(settingRepository.GetSetting(API_KEY_SETTING, DEFAULT_API_KEY));
        return true;
    }

    auto ConfigApiSettingsProvider::Has() -> bool
    {
        const auto settings = this->Get();
        return !settings.Key().empty();
    }
} // namespace UKControllerPluginUtils::Api
