#include "ApiSettings.h"
#include "ConfigApiSettingsProvider.h"
#include "setting/SettingRepositoryInterface.h"

using UKControllerPlugin::Setting::SettingRepositoryInterface;

namespace UKControllerPluginUtils::Api {

    ConfigApiSettingsProvider::ConfigApiSettingsProvider(SettingRepositoryInterface& configProvider)
        : configProvider(configProvider)
    {
    }

    void ConfigApiSettingsProvider::SaveSettings(const ApiSettings& settings){
        configProvider.Se
    }

    std::unique_ptr<ApiSettings> ConfigApiSettingsProvider::LoadSettings()
    {
        return std::make_unique<ApiSettings>(
            configProvider.GetSetting("api-url", "https://ukcp.vatsim.uk"), configProvider.GetSetting("api-key"));
    }

    std::unique_ptr<ApiSettings> ConfigApiSettingsProvider::NewSettings()
    {
        return nullptr;
    }
} // namespace UKControllerPluginUtils::Api
