#pragma once

namespace UKControllerPlugin {
    namespace Euroscope {
        class UserSetting;
        class UserSettingAwareCollection;
    } // namespace Euroscope
    namespace Setting {
        class SettingRepository;
    } // namespace Setting
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Euroscope {

    /*
        An ATL dialog for the plugins general settings.
    */
    class GeneralSettingsDialog
    {
        public:
        GeneralSettingsDialog(
            UKControllerPlugin::Euroscope::UserSetting& userSettings,
            const UKControllerPlugin::Euroscope::UserSettingAwareCollection& userSettingsHandlers,
            Setting::SettingRepository& settings);
        GeneralSettingsDialog(const GeneralSettingsDialog& newObject);

        static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

        private:
        LRESULT _WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
        bool GetCheckboxStateFromSettings(std::string setting);
        bool GetSettingFromCheckboxState(HWND hwnd, int checkboxId);
        LRESULT InitDialog(HWND hwnd);
        void DestroyDialog(HWND hwnd);

        const std::map<std::string, std::wstring> releaseChannelMap{
            {"stable", L"Stable"},
            {"beta", L"Beta"},
        };

        const std::string DEFAULT_RELEASE_CHANNEL = "stable";

        // A place where user settings are retrieved and stored
        UKControllerPlugin::Euroscope::UserSetting& userSettings;

        // A set of handlers that want to know when user settings get updated
        const UKControllerPlugin::Euroscope::UserSettingAwareCollection& userSettingsHandlers;

        // For global settings that transcend ES profiles
        Setting::SettingRepository& settings;
    };
} // namespace UKControllerPlugin::Euroscope
