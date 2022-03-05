#pragma once

namespace UKControllerPlugin {
    namespace Setting {
        class SettingRepository;
    } // namespace Setting
    namespace Windows {
        class WinApiInterface;
    } // namespace Windows
} // namespace UKControllerPlugin

namespace UKControllerPluginUtils::Update {
    void BootstrapReleaseChannelSettings(
        UKControllerPlugin::Setting::SettingRepository& settings,
        UKControllerPlugin::Windows::WinApiInterface& windows);
} // namespace UKControllerPluginUtils::Update
