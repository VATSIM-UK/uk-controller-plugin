#include "FirstTimeApiConfigLoader.h"
#include "api/ApiSettingsProviderInterface.h"

namespace UKControllerPlugin::Api {

    void LocateConfig(UKControllerPluginUtils::Api::ApiSettingsProviderInterface& settingsProvider)
    {
        if (settingsProvider.Has()) {
            LogInfo("Api configuration successfully loaded");
            return;
        }

        if (!settingsProvider.Reload()) {
            LogInfo("First time api config load, user elected not to load config");
            return;
        }

        LogInfo("First time api config load completed");
    }
} // namespace UKControllerPlugin::Api
