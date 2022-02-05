#include "ApiSettings.h"
#include "ConfigApiSettingsProvider.h"
#include "setting/SettingRepositoryInterface.h"

using UKControllerPlugin::Setting::SettingRepositoryInterface;

namespace UKControllerPluginUtils::Api {

    ConfigApiSettingsProvider::ConfigApiSettingsProvider(SettingRepositoryInterface& settingRepository)
        : settingRepository(settingRepository)
    {
    }

    auto ConfigApiSettingsProvider::Get() -> ApiSettings&
    {
        if (!this->settings) {
            this->settings = std::make_unique<ApiSettings>(
                settingRepository.GetSetting(API_URL_SETTING, DEFAULT_API_URL),
                settingRepository.GetSetting(API_KEY_SETTING));
        }

        return *this->settings;
    }

    void ConfigApiSettingsProvider::Reload()
    {
        this->settingRepository.ReloadSetting(API_KEY_SETTING);
        this->settingRepository.ReloadSetting(API_URL_SETTING);

        this->settings->Url(settingRepository.GetSetting(API_URL_SETTING, DEFAULT_API_URL));
        this->settings->Key(settingRepository.GetSetting(API_KEY_SETTING));
    }
} // namespace UKControllerPluginUtils::Api
