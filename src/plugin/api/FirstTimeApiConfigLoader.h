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
    auto LocateConfig(
        const Dialog::DialogManager& dialogManager,
        Windows::WinApiInterface& windows,
        UKControllerPluginUtils::Api::ApiSettingsProviderInterface& settingsProvider) -> bool;
} // namespace UKControllerPlugin::Api
