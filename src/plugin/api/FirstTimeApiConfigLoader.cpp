#include "FirstTimeApiConfigLoader.h"
#include "api/ApiSettingsProviderInterface.h"

namespace UKControllerPlugin::Api {

    auto LocateConfig(UKControllerPluginUtils::Api::ApiSettingsProviderInterface& settingsProvider) -> bool
    {
        if (settingsProvider.Has()) {
            LogInfo("Api configuration successfully loaded");
            return true;
        }

        if (!settingsProvider.Reload()) {
            LogInfo("First time api config load, user elected not to load config");
            return false;
        }

        LogInfo("First time api config load completed");
        return true;
    }
} // namespace UKControllerPlugin::Api
