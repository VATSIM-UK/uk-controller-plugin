#pragma once
#include "ApiSettingsProviderInterface.h"

namespace UKControllerPlugin::Setting {
    class SettingRepositoryInterface;
}

namespace UKControllerPluginUtils::Api {
    
    class ConfigApiSettingsProvider : public ApiSettingsProviderInterface
    {
        public:
        ConfigApiSettingsProvider(UKControllerPlugin::Setting::SettingRepositoryInterface& settingRepository);
        [[nodiscard]] auto Get() -> ApiSettings& override;
        void Reload() override;

        private:
        // Provides us config
        UKControllerPlugin::Setting::SettingRepositoryInterface& settingRepository;
        
        // The owned setting object
        std::unique_ptr<ApiSettings> settings;

        // Some setting keys
        const std::string API_KEY_SETTING = "api-key";
        const std::string API_URL_SETTING = "api-url";
        const std::string DEFAULT_API_URL = "https://ukcp.vatsim.uk";
    };
} // namespace UKControllerPluginUtils::Api
