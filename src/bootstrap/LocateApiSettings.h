#pragma once

namespace UKControllerPlugin {
    namespace Windows {
        class WinApiInterface;
    }  // namespace Windows
    namespace Setting {
        class SettingRepository;
    }  // namespace Setting
}  // namespace UKControllerPlugin


namespace UKControllerPlugin {
namespace Bootstrap {

    void LocateApiSettings(
        UKControllerPlugin::Windows::WinApiInterface & winApi,
        UKControllerPlugin::Setting::SettingRepository & settings
    );

}  // namespace Bootstrap
}  // namespace UKControllerPlugin
