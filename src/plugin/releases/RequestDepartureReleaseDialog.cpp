#include "releases/RequestDepartureReleaseDialog.h"
#include "DepartureReleaseEventHandler.h"
#include "dialog/DialogCallArgument.h"
#include "controller/ActiveCallsign.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPosition.h"
#include "euroscope/EuroScopeCControllerInterface.h"
#include "helper/HelperFunctions.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"

namespace UKControllerPlugin {
    namespace Releases {

        RequestDepartureReleaseDialog::RequestDepartureReleaseDialog(
            const std::shared_ptr<DepartureReleaseEventHandler> eventHandler,
            const Controller::ActiveCallsignCollection& activeCallsigns,
            Euroscope::EuroscopePluginLoopbackInterface& plugin)
            : activeCallsigns(activeCallsigns), plugin(plugin), eventHandler(std::move(eventHandler))
        {
        }

        /*
            Private dialog procedure for the dialog, should be used
            against a bound instance.
        */
        LRESULT RequestDepartureReleaseDialog::_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
                    this->RequestRelease(hwnd);
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
        LRESULT RequestDepartureReleaseDialog::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            if (msg == WM_INITDIALOG) {
                LogInfo("Request departure releases dialog opened");
                SetWindowLongPtr(
                    hwnd, GWLP_USERDATA, reinterpret_cast<Dialog::DialogCallArgument*>(lParam)->dialogArgument);
            } else if (msg == WM_DESTROY) {
                SetWindowLongPtr(hwnd, GWLP_USERDATA, NULL);
                LogInfo("Request departure releases dialog closed");
            }

            RequestDepartureReleaseDialog* dialog =
                reinterpret_cast<RequestDepartureReleaseDialog*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            return dialog ? dialog->_WndProc(hwnd, msg, wParam, lParam) : FALSE;
        }

        /*
         * Set up the notifications list
         */
        void RequestDepartureReleaseDialog::InitDialog(HWND hwnd, LPARAM lParam)
        {
            // Get the aircraft callsign from dialog argument
            this->selectedCallsign = std::string(
                reinterpret_cast<const char*>(reinterpret_cast<Dialog::DialogCallArgument*>(lParam)->contextArgument));

            // Set the aircraft callsign box
            std::wstring wideCallsign = HelperFunctions::ConvertToWideString(this->selectedCallsign);

            SendDlgItemMessage(
                hwnd,
                IDC_DEPARTURE_RELEASE_REQUEST_CALLSIGN,
                WM_SETTEXT,
                NULL,
                reinterpret_cast<LPARAM>(wideCallsign.c_str()));

            // Populate the controller list
            this->plugin.ApplyFunctionToAllControllers(
                [this, &hwnd](std::shared_ptr<Euroscope::EuroScopeCControllerInterface> controller) {
                    // If the controller is the user, or isn't an active callsign, don't include
                    if (controller->IsCurrentUser() ||
                        !this->activeCallsigns.CallsignActive(controller->GetCallsign())) {
                        return;
                    }

                    auto normalisedPosition =
                        this->activeCallsigns.GetCallsign(controller->GetCallsign()).GetNormalisedPosition();

                    // If the position can't receive releases, don't include
                    if (!normalisedPosition.ReceivesDepartureReleases()) {
                        return;
                    }

                    std::wstring callsign = HelperFunctions::ConvertToWideString(controller->GetCallsign());

                    // Set the data
                    int itemIndex = SendDlgItemMessage(
                        hwnd,
                        IDC_DEPARTURE_RELEASE_TARGET_CONTROLLER,
                        CB_ADDSTRING,
                        NULL,
                        reinterpret_cast<LPARAM>(callsign.c_str()));

                    int controllerPositionId = normalisedPosition.GetId();
                    SendDlgItemMessage(
                        hwnd, IDC_DEPARTURE_RELEASE_TARGET_CONTROLLER, CB_SETITEMDATA, itemIndex, controllerPositionId);
                });

            SendDlgItemMessage(hwnd, IDC_DEPARTURE_RELEASE_TARGET_CONTROLLER, CB_SETCURSEL, 0, NULL);
        }

        void RequestDepartureReleaseDialog::RequestRelease(HWND hwnd)
        {
            int selectedController =
                SendDlgItemMessage(hwnd, IDC_DEPARTURE_RELEASE_TARGET_CONTROLLER, CB_GETCURSEL, NULL, NULL);

            // If no controller selected, stop
            if (selectedController == CB_ERR) {
                return;
            }

            // Get the controller and make a release request
            int controllerId = SendDlgItemMessage(
                hwnd, IDC_DEPARTURE_RELEASE_TARGET_CONTROLLER, CB_GETITEMDATA, selectedController, NULL);

            this->eventHandler->RequestRelease(this->selectedCallsign, controllerId);
        }

    } // namespace Releases
} // namespace UKControllerPlugin
