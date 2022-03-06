#pragma once

namespace UKControllerPluginUtils::Api {
    class ApiSettingsProviderInterface;
} // namespace UKControllerPluginUtils::Api

namespace UKControllerPlugin::Api {
    auto LocateConfig(UKControllerPluginUtils::Api::ApiSettingsProviderInterface& settingsProvider) -> bool;
} // namespace UKControllerPlugin::Api
