#include "pch/stdafx.h"
#include "countdown/TimerConfigurationDialog.h"
#include "dialog/DialogCallArgument.h"

using UKControllerPlugin::Dialog::DialogCallArgument;

namespace UKControllerPlugin {
    namespace Countdown {

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

        LRESULT TimerConfigurationDialog::_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            return FALSE;
        }
    }  // namespace Countdown
}  // namespace UKControllerPlugin
