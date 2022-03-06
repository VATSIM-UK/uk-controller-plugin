#pragma once

namespace UKControllerPluginUtils::Api {
    class ApiSettingsProviderInterface;
} // namespace UKControllerPluginUtils::Api

namespace UKControllerPlugin::Windows {
    class WinApiInterface;
} // namespace UKControllerPlugin::Windows
namespace UKControllerPlugin::Api {
    void FirstTimeApiAuthorisationCheck(
        UKControllerPluginUtils::Api::ApiSettingsProviderInterface& settingsProviderInterface,
        Windows::WinApiInterface& windows);
} // namespace UKControllerPlugin::Api
