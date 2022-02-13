#include "ApiSettings.h"
#include "ApiSettingsFactory.h"
#include "setting/SettingRepositoryInterface.h"

using UKControllerPlugin::Setting::SettingRepositoryInterface;

namespace UKControllerPluginUtils::Api {

    std::shared_ptr<ApiSettings> apiSettings = nullptr;

    auto Make(const SettingRepositoryInterface& settings) -> std::shared_ptr<ApiSettings>
    {
        if (apiSettings == nullptr) {
            apiSettings = std::make_shared<ApiSettings>(
                settings.GetSetting("api-url", "https://ukcp.vatsim.uk"), settings.GetSetting("api-key", ""));
        }

        return apiSettings;
    }
} // namespace UKControllerPluginUtils::Api
