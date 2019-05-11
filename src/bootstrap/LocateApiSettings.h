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

    bool ReplaceApiSettings(UKControllerPlugin::Windows::WinApiInterface & winApi);
    void UserRequestedKeyUpdate(UKControllerPlugin::Windows::WinApiInterface & winApi);

}  // namespace Bootstrap
}  // namespace UKControllerPlugin
