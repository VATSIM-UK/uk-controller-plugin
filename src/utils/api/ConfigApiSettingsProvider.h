#pragma once
#include "ApiSettingsProviderInterface.h"

namespace UKControllerPlugin::Setting {
    class SettingRepositoryInterface;
}

namespace UKControllerPluginUtils::Api {
    class ConfigApiSettingsProvider : public ApiSettingsProviderInterface
    {
        public:
        ConfigApiSettingsProvider(UKControllerPlugin::Setting::SettingRepositoryInterface& configProvider);
        void SaveSettings(const ApiSettings& settings) override;
        [[nodiscard]] auto LoadSettings() -> std::unique_ptr<ApiSettings> override;
        [[nodiscard]] auto NewSettings() -> std::unique_ptr<ApiSettings> override;

        private:
        
        // Provides us the JSON config
        UKControllerPlugin::Settings::SettingProviderInterface& configProvider;
    };
} //
