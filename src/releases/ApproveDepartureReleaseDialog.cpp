#include "pch/stdafx.h"
#include "releases/ApproveDepartureReleaseDialog.h"
#include "dialog/DialogCallArgument.h"

namespace UKControllerPlugin {
    namespace Releases {

        /*
            Private dialog procedure for the dialog, should be used
            against a bound instance.
        */
        LRESULT ApproveDepartureReleaseDialog::_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            switch (msg) {
                // Initialise
                case WM_INITDIALOG: {
                    this->InitDialog(hwnd, lParam);
                    return TRUE;
                };
                // Window closed
                case WM_CLOSE: {
                    EndDialog(hwnd, wParam);
                    return TRUE;
                }
                // Buttons pressed
                case WM_COMMAND: {
                    switch (LOWORD(wParam)) {
                        case IDOK: {
                            EndDialog(hwnd, wParam);
                            return TRUE;
                        }
                        default:
                            return FALSE;
                    }
                }
            }
            return FALSE;
        }

        /*
            Public dialog procedure for the dialog
        */
        LRESULT ApproveDepartureReleaseDialog::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            if (msg == WM_INITDIALOG) {
                LogInfo("Approve departure releases dialog opened");
                SetWindowLongPtr(
                    hwnd,
                    GWLP_USERDATA,
                    reinterpret_cast<Dialog::DialogCallArgument *>(lParam)->dialogArgument
                );
            } else if (msg == WM_DESTROY) {
                SetWindowLongPtr(hwnd, GWLP_USERDATA, NULL);
                LogInfo("Approve departure releases dialog closed");
            }

            ApproveDepartureReleaseDialog* dialog = reinterpret_cast<ApproveDepartureReleaseDialog*>(
                GetWindowLongPtr(hwnd, GWLP_USERDATA)
            );
            return dialog ? dialog->_WndProc(hwnd, msg, wParam, lParam) : FALSE;
        }

        /*
         * Set up the notifications list
         */
        void ApproveDepartureReleaseDialog::InitDialog(HWND hwnd, LPARAM lParam)
        { }

    } // namespace Releases
}  // namespace UKControllerPlugin
