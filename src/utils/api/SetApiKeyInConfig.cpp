#include "SetApiKeyInConfig.h"
#include "setting/SettingRepositoryInterface.h"

namespace UKControllerPluginUtils::Api {

    SetApiKeyInConfig::SetApiKeyInConfig(UKControllerPlugin::Setting::SettingRepositoryInterface& settingProvider)
        : settingProvider(settingProvider)
    {
    }

    void SetApiKeyInConfig::OnEvent(const ApiKeyReceivedEvent& event)
    {
        settingProvider.UpdateSetting("api-key", event.key);
    }
} // namespace UKControllerPluginUtils::Api
