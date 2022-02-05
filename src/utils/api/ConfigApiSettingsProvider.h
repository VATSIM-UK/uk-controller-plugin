#pragma once
#include "ApiSettingsProviderInterface.h"

namespace UKControllerPlugin {
    namespace Setting {
        class SettingRepositoryInterface;
    } //
    namespace Windows {
        class WinApiInterface;
    }
}

namespace UKControllerPluginUtils::Api {
    
    class ConfigApiSettingsProvider : public ApiSettingsProviderInterface
    {
        public:
        ConfigApiSettingsProvider(
            UKControllerPlugin::Setting::SettingRepositoryInterface& settingRepository,
            UKControllerPlugin::Windows::WinApiInterface& windows
        );
        [[nodiscard]] auto Get() -> ApiSettings& override;
        void Reload() override;

        private:
        // Provides us config
        UKControllerPlugin::Setting::SettingRepositoryInterface& settingRepository;
        
        // For reloading
        UKControllerPlugin::Windows::WinApiInterface& windows;
        
        // The owned setting object
        std::unique_ptr<ApiSettings> settings;

        // Some setting keys
        const std::string API_KEY_SETTING = "api-key";
        const std::string API_URL_SETTING = "api-url";
        const std::string DEFAULT_API_URL = "https://ukcp.vatsim.uk";
    };
} // namespace UKControllerPluginUtils::Api
