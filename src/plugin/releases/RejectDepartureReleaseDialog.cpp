#include "DepartureReleaseEventHandler.h"
#include "DepartureReleaseRequest.h"
#include "RejectDepartureReleaseDialog.h"
#include "dialog/DialogCallArgument.h"
#include "helper/HelperFunctions.h"

namespace UKControllerPlugin {
    namespace Releases {

        RejectDepartureReleaseDialog::RejectDepartureReleaseDialog(
            std::shared_ptr<DepartureReleaseEventHandler> eventHandler)
            : eventHandler(std::move(eventHandler))
        {
        }

        /*
    Private dialog procedure for the dialog, should be used
    against a bound instance.
*/
        LRESULT RejectDepartureReleaseDialog::_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
                    this->RejectRelease(hwnd);
                    EndDialog(hwnd, wParam);
                    return TRUE;
                }
                case IDCANCEL: {
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
        LRESULT RejectDepartureReleaseDialog::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            if (msg == WM_INITDIALOG) {
                LogInfo("Reject departure releases dialog opened");
                SetWindowLongPtr(
                    hwnd, GWLP_USERDATA, reinterpret_cast<Dialog::DialogCallArgument*>(lParam)->dialogArgument);
            } else if (msg == WM_DESTROY) {
                SetWindowLongPtr(hwnd, GWLP_USERDATA, NULL);
                LogInfo("Reject departure releases dialog closed");
            }

            RejectDepartureReleaseDialog* dialog =
                reinterpret_cast<RejectDepartureReleaseDialog*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            return dialog ? dialog->_WndProc(hwnd, msg, wParam, lParam) : FALSE;
        }

        /*
         * Set up the notifications list
         */
        void RejectDepartureReleaseDialog::InitDialog(HWND hwnd, LPARAM lParam)
        {
            // Get the aircraft callsign from dialog argument
            this->selectedRelease = *reinterpret_cast<std::shared_ptr<DepartureReleaseRequest>*>(
                reinterpret_cast<Dialog::DialogCallArgument*>(lParam)->contextArgument);

            // Max length of the remarks field
            SendDlgItemMessage(hwnd, IDC_RELEASE_REJECT_REMARKS, EM_SETLIMITTEXT, REMARKS_BUFFER_SIZE - 1, NULL);
        }

        void RejectDepartureReleaseDialog::RejectRelease(HWND hwnd)
        {
            // Get the remarks
            TCHAR remarks[REMARKS_BUFFER_SIZE];
            SendDlgItemMessage(
                hwnd, IDC_RELEASE_REJECT_REMARKS, WM_GETTEXT, REMARKS_BUFFER_SIZE, reinterpret_cast<LPARAM>(remarks));

            // Reject the release
            eventHandler->RejectRelease(this->selectedRelease->Id(), HelperFunctions::ConvertToRegularString(remarks));
        }
    } // namespace Releases
} // namespace UKControllerPlugin
