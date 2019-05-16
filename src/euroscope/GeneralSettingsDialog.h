#pragma once

namespace UKControllerPlugin {
    namespace Euroscope {
        class UserSetting;
        class UserSettingAwareCollection;
    }  // namespace Euroscope
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Euroscope {

        /*
            An ATL dialog for the plugins general settings.
        */
        class GeneralSettingsDialog
        {
            public:

                GeneralSettingsDialog(
                    UKControllerPlugin::Euroscope::UserSetting & userSettings,
                    const UKControllerPlugin::Euroscope::UserSettingAwareCollection & userSettingsHandlers
                );
                GeneralSettingsDialog(const GeneralSettingsDialog & newObject);

                static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

            private:

                LRESULT _WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
                bool GetCheckboxStateFromSettings(std::string setting);
                bool GetSettingFromCheckboxState(HWND hwnd, int checkboxId);
                LRESULT InitDialog(HWND hwnd);
                void DestroyDialog(HWND hwnd);

                // A place where user settings are retrieved and stored
                UKControllerPlugin::Euroscope::UserSetting & userSettings;

                // A set of handlers that want to know when user settings get updated
                const UKControllerPlugin::Euroscope::UserSettingAwareCollection & userSettingsHandlers;
        };
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
