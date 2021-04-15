#include "pch/stdafx.h"
#include "euroscope/GeneralSettingsDialog.h"
#include "euroscope/UserSetting.h"
#include "euroscope/GeneralSettingsEntries.h"
#include "euroscope/UserSettingAwareCollection.h"
#include "dialog/DialogCallArgument.h"

using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Euroscope::GeneralSettingsEntries;
using UKControllerPlugin::Euroscope::UserSettingAwareCollection;
using UKControllerPlugin::Dialog::DialogCallArgument;

namespace UKControllerPlugin {
    namespace Euroscope {

        GeneralSettingsDialog::GeneralSettingsDialog(
            UserSetting & userSettings,
            const UserSettingAwareCollection & userSettingsHandlers
        )
            : userSettings(userSettings), userSettingsHandlers(userSettingsHandlers)
        {

        }

        GeneralSettingsDialog::GeneralSettingsDialog(const GeneralSettingsDialog & newObject)
            : userSettings(newObject.userSettings), userSettingsHandlers(newObject.userSettingsHandlers)
        {
        }

        LRESULT GeneralSettingsDialog::InitDialog(HWND hwnd)
        {
            CheckDlgButton(
                hwnd,
                GS_DIALOG_PRENOTE_CHECK,
                this->GetCheckboxStateFromSettings(GeneralSettingsEntries::usePrenoteSettingsKey)
            );

            CheckDlgButton(
                hwnd,
                GS_DIALOG_IA_CHECK,
                this->GetCheckboxStateFromSettings(GeneralSettingsEntries::initialAltitudeToggleSettingsKey)
            );

            CheckDlgButton(
                hwnd,
                GS_DIALOG_IH_CHECK,
                this->GetCheckboxStateFromSettings(GeneralSettingsEntries::initialHeadingToggleSettingsKey)
            );

            CheckDlgButton(
                hwnd,
                GS_DIALOG_SQUAWK_CHECK,
                this->GetCheckboxStateFromSettings(GeneralSettingsEntries::squawkToggleSettingsKey)
            );

            CheckDlgButton(
                hwnd,
                GS_DIALOG_QNH_CHECK,
                this->GetCheckboxStateFromSettings(GeneralSettingsEntries::pressureMonitorSendMessageKey)
            );

            CheckDlgButton(
                hwnd,
                GS_TIME_FORMAT_CHECK,
                this->GetCheckboxStateFromSettings(GeneralSettingsEntries::unknownTimeFormatBlankKey)
            );

            return TRUE;
        }

        void GeneralSettingsDialog::DestroyDialog(HWND hwnd)
        {
            LogInfo("General settings dialog saved");

            // Prenotes Toggle
            this->userSettings.Save(
                GeneralSettingsEntries::usePrenoteSettingsKey,
                GeneralSettingsEntries::usePrenoteSettingsDescription,
                this->GetSettingFromCheckboxState(hwnd, GS_DIALOG_PRENOTE_CHECK)
            );

            // Initial Altitudes Toggle
            this->userSettings.Save(
                GeneralSettingsEntries::initialAltitudeToggleSettingsKey,
                GeneralSettingsEntries::initialAltitudeToggleSettingsDescription,
                this->GetSettingFromCheckboxState(hwnd, GS_DIALOG_IA_CHECK)
            );

            // Initial Headings Toggle
            this->userSettings.Save(
                GeneralSettingsEntries::initialHeadingToggleSettingsKey,
                GeneralSettingsEntries::initialHeadingToggleSettingsDescription,
                this->GetSettingFromCheckboxState(hwnd, GS_DIALOG_IH_CHECK)
            );

            // Squawk Toggle
            this->userSettings.Save(
                GeneralSettingsEntries::squawkToggleSettingsKey,
                GeneralSettingsEntries::squawkToggleSettingsDescription,
                this->GetSettingFromCheckboxState(hwnd, GS_DIALOG_SQUAWK_CHECK)
            );

            // Pressure monitor
            this->userSettings.Save(
                GeneralSettingsEntries::pressureMonitorSendMessageKey,
                GeneralSettingsEntries::pressureMonitorSendMessageDescription,
                this->GetSettingFromCheckboxState(hwnd, GS_DIALOG_QNH_CHECK)
            );

            // Time format
            this->userSettings.Save(
                GeneralSettingsEntries::unknownTimeFormatBlankKey,
                GeneralSettingsEntries::unknownTimeFormatBlankDescription,
                this->GetSettingFromCheckboxState(hwnd, GS_TIME_FORMAT_CHECK)
            );

            this->userSettingsHandlers.UserSettingsUpdateEvent(this->userSettings);
        }

        /*
            Public procedure that windows calls directly
        */
        LRESULT GeneralSettingsDialog::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            if (msg == WM_INITDIALOG) {
                LogInfo("General settings dialog opened");
                SetWindowLongPtr(
                    hwnd,
                    GWLP_USERDATA,
                    reinterpret_cast<DialogCallArgument *>(lParam)->dialogArgument
                );
            } else if (msg == WM_DESTROY) {
                SetWindowLongPtr(hwnd, GWLP_USERDATA, NULL);
                LogInfo("General settings dialog closed");
            }

            GeneralSettingsDialog * dialog = reinterpret_cast<GeneralSettingsDialog*>(
                GetWindowLongPtr(hwnd, GWLP_USERDATA)
            );
            return dialog ? dialog->_WndProc(hwnd, msg, wParam, lParam) : FALSE;
        }

        /*
            Private procedure that handles calls on the object
        */
        LRESULT GeneralSettingsDialog::_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            switch (msg) {
                // Initialise
                case WM_INITDIALOG: {
                    this->InitDialog(hwnd);
                    return TRUE;
                };
                // Window Closed
                case WM_CLOSE: {
                    EndDialog(hwnd, wParam);
                    return TRUE;
                }
                // Buttons pressed
                case WM_COMMAND: {
                    switch (LOWORD(wParam)) {
                        case IDCANCEL: {
                            EndDialog(hwnd, wParam);
                            return TRUE;
                        }
                        case IDOK: {
                            // OK clicked, close the window
                            this->DestroyDialog(hwnd);
                            EndDialog(hwnd, wParam);
                            return TRUE;
                        }
                    }
                }
            }

            return FALSE;
        }

        /*
            Get the appropriate checkbox state from settings.
        */
        bool GeneralSettingsDialog::GetCheckboxStateFromSettings(std::string setting)
        {
            return this->userSettings.GetBooleanEntry(setting) ? BST_CHECKED : BST_UNCHECKED;
        }

        /*
            Get the setting to save based on the checkbox value
        */
        bool GeneralSettingsDialog::GetSettingFromCheckboxState(HWND hwnd, int checkboxId)
        {
            return IsDlgButtonChecked(hwnd, checkboxId) == BST_CHECKED;
        }
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
