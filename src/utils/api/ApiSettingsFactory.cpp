#include "ApiSettings.h"
#include "ApiSettingsFactory.h"
#include "setting/SettingRepository.h"

using UKControllerPlugin::Setting::SettingRepository;

namespace UKControllerPluginUtils::Api {

    std::shared_ptr<ApiSettings> apiSettings = nullptr;

    auto Make(const SettingRepository& settings) -> std::shared_ptr<ApiSettings>
    {
        if (apiSettings == nullptr) {
            apiSettings = std::make_shared<ApiSettings>(
                settings.GetSetting("api-url", "https://ukcp.vatsim.uk"), settings.GetSetting("api-key"));
        }

        return apiSettings;
    }
} // namespace UKControllerPluginUtils::Api
