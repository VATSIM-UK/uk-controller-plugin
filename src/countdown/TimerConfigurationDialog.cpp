#include "pch/stdafx.h"
#include "countdown/TimerConfigurationDialog.h"
#include "dialog/DialogCallArgument.h"
#include "countdown/GlobalCountdownSettingFunctions.h"

using UKControllerPlugin::Dialog::DialogCallArgument;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Euroscope::UserSettingAwareCollection;
using UKControllerPlugin::Countdown::GetTimerDurationKey;
using UKControllerPlugin::Countdown::GetTimerEnabledKey;

namespace UKControllerPlugin {
    namespace Countdown {
        TimerConfigurationDialog::TimerConfigurationDialog(
            UserSetting & userSettings,
            const UserSettingAwareCollection & userSettingHandlers
        ) : userSettings(userSettings), userSettingHandlers(userSettingHandlers)
        {
        }
        
        /*
            Public facing window procedure
        */
        LRESULT TimerConfigurationDialog::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            if (msg == WM_INITDIALOG) {
                LogInfo("Timer configuration dialog opened");
                SetWindowLongPtr(
                    hwnd,
                    GWLP_USERDATA,
                    reinterpret_cast<DialogCallArgument *>(lParam)->dialogArgument
                );
            } else if (msg == WM_DESTROY) {
                SetWindowLongPtr(hwnd, GWLP_USERDATA, NULL);
                LogInfo("Timer configuration dialog closed");
            }

            TimerConfigurationDialog * dialog = reinterpret_cast<TimerConfigurationDialog*>(
                GetWindowLongPtr(hwnd, GWLP_USERDATA)
            );
            return dialog ? dialog->_WndProc(hwnd, msg, wParam, lParam) : FALSE;
        }

        /*
            Private window procedure bound to the objects
        */
        LRESULT TimerConfigurationDialog::_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            switch (msg) {
                // Initialise
                case WM_INITDIALOG: {
                    this->InitDialog(hwnd, lParam);
                    return TRUE;
                };
                // Hold Window Closed
                case WM_CLOSE: {
                    EndDialog(hwnd, wParam);
                    return TRUE;
                }
                // Buttons pressed
                case WM_COMMAND: {
                    switch (LOWORD(wParam)) {
                        case IDOK: {
                            this->SaveDialog(hwnd);
                            EndDialog(hwnd, wParam);
                            return TRUE;
                        }
                        case IDCANCEL: {
                            EndDialog(hwnd, wParam);
                            return TRUE;
                        }
                    }
                }
            }

            return FALSE;
        }

        /*
            Initialise the timer controls from user settings
        */
        void TimerConfigurationDialog::InitDialog(HWND hwnd, LPARAM lParam)
        {
            this->InitTimerRow(
                hwnd,
                1,
                TIMER1_ENABLED_CHECK,
                TIMER1_EDIT_DURATION
            );

            this->InitTimerRow(
                hwnd,
                2,
                TIMER2_ENABLED_CHECK,
                TIMER2_EDIT_DURATION
            );

            this->InitTimerRow(
                hwnd,
                3,
                TIMER3_ENABLED_CHECK,
                TIMER3_EDIT_DURATION
            );

            this->InitTimerRow(
                hwnd,
                4,
                TIMER4_ENABLED_CHECK,
                TIMER4_EDIT_DURATION
            );

            this->InitTimerRow(
                hwnd,
                5,
                TIMER5_ENABLED_CHECK,
                TIMER5_EDIT_DURATION
            );
        }

        /*
            Save each of the timer settings and notify the
            user settings aware classes that things have changed.
        */
        void TimerConfigurationDialog::SaveDialog(HWND hwnd)
        {
            this->SaveTimerRow(
                hwnd,
                1,
                TIMER1_ENABLED_CHECK,
                TIMER1_EDIT_DURATION
            );

            this->SaveTimerRow(
                hwnd,
                2,
                TIMER2_ENABLED_CHECK,
                TIMER2_EDIT_DURATION
            );

            this->SaveTimerRow(
                hwnd,
                3,
                TIMER3_ENABLED_CHECK,
                TIMER3_EDIT_DURATION
            );

            this->SaveTimerRow(
                hwnd,
                4,
                TIMER4_ENABLED_CHECK,
                TIMER4_EDIT_DURATION
            );

            this->SaveTimerRow(
                hwnd,
                5,
                TIMER5_ENABLED_CHECK,
                TIMER5_EDIT_DURATION
            );

            this->userSettingHandlers.UserSettingsUpdateEvent(userSettings);
        }

        /*
            Setup the row for one timer
        */
        void TimerConfigurationDialog::InitTimerRow(
            HWND hwnd,
            unsigned int timerId,
            unsigned int enabledCheckboxId,
            unsigned int durationBoxId
        ) {
            CheckDlgButton(
                hwnd,
                enabledCheckboxId,
                this->userSettings.GetBooleanEntry(GetTimerEnabledKey(timerId), true) ? BST_CHECKED : BST_UNCHECKED
            );

            SendDlgItemMessage(
                hwnd,
                durationBoxId,
                WM_SETTEXT,
                NULL,
                reinterpret_cast<LPARAM>(
                    std::to_wstring(this->userSettings.GetUnsignedIntegerEntry(
                        GetTimerDurationKey(timerId), 60)
                    ).c_str()
                )
            );

            SendDlgItemMessage(
                hwnd,
                durationBoxId,
                EM_SETLIMITTEXT,
                3,
                NULL
            );
        }

        /*
            Save each row of the timer dialog
        */
        void TimerConfigurationDialog::SaveTimerRow(
            HWND hwnd,
            unsigned int timerId,
            unsigned int enabledCheckboxId,
            unsigned int durationBoxId
        ) {
            // Get the timer duration
            TCHAR buffer[4];
            LRESULT valid = SendDlgItemMessage(
                hwnd,
                durationBoxId,
                WM_GETTEXT,
                4,
                reinterpret_cast<LPARAM>(buffer)
            );

            if (!valid) {
                LogWarning("Unable to read duration for timer #" + std::to_string(timerId));
                return;
            }

            unsigned int timerDuration = std::stoi(buffer);

            // Check timer duration valid
            if (timerDuration > 999) {
                LogWarning("Invalid timer duration " + std::to_string(timerDuration)
                    + " for timer #" + std::to_string(timerId));
                return;
            }

            // Save the setting
            this->userSettings.Save(
                GetTimerEnabledKey(timerId),
                GetTimerEnabledDescription(timerId),
                IsDlgButtonChecked(hwnd, enabledCheckboxId) == BST_CHECKED
            );

            this->userSettings.Save(
                GetTimerDurationKey(timerId),
                GetTimerDurationDescription(timerId),
                timerDuration
            );
        }
    }  // namespace Countdown
}  // namespace UKControllerPlugin
