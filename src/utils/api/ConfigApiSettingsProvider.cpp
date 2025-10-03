#include "ApiSettings.h"
#include "ConfigApiSettingsProvider.h"
#include "setting/SettingRepositoryInterface.h"

using UKControllerPlugin::Setting::SettingRepositoryInterface;

namespace UKControllerPluginUtils::Api {

    ConfigApiSettingsProvider::ConfigApiSettingsProvider(SettingRepositoryInterface& settingRepository)
        : settingRepository(settingRepository)
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

    auto ConfigApiSettingsProvider::Has() -> bool
    {
        const auto& currentSettings = this->Get();
        return !currentSettings.Key().empty();
    }
} // namespace UKControllerPluginUtils::Api
