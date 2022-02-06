#pragma once

namespace UKControllerPluginUtils::Api {
    class ApiSettingsProviderInterface;
} // namespace UKControllerPluginUtils::Api

namespace UKControllerPlugin::Api {
    void LocateConfig(UKControllerPluginUtils::Api::ApiSettingsProviderInterface& settingsProvider);
} // namespace UKControllerPlugin::Api
