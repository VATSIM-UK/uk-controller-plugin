#pragma once

namespace UKControllerPluginUtils::Api {
    class ApiSettingsProviderInterface;
} // namespace UKControllerPluginUtils::Api

namespace UKControllerPlugin {
    namespace Dialog {
        class DialogManager;
    } // namespace Dialog
    namespace Windows {
        class WinApiInterface;
    } // namespace Windows
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Api {
    void FirstTimeApiAuthorisationCheck(
        UKControllerPluginUtils::Api::ApiSettingsProviderInterface& settingsProviderInterface,
        Windows::WinApiInterface& windows,
        const Dialog::DialogManager& dialogManager);
} // namespace UKControllerPlugin::Api
