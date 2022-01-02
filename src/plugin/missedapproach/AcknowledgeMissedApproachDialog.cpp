#include "AcknowledgeMissedApproachDialog.h"
#include "AcknowledgeMissedApproachTagFunction.h"
#include "dialog/DialogCallArgument.h"
#include "helper/HelperFunctions.h"

using UKControllerPlugin::HelperFunctions;
using UKControllerPlugin::Dialog::DialogCallArgument;

namespace UKControllerPlugin::MissedApproach {

    AcknowledgeMissedApproachDialog::AcknowledgeMissedApproachDialog(
        std::shared_ptr<AcknowledgeMissedApproachTagFunction> tagFunction)
        : tagFunction(std::move(tagFunction))
    {
    }

    auto AcknowledgeMissedApproachDialog::InitDialog(HWND hwnd, LPARAM lParam) -> LRESULT
    {
        SendDlgItemMessage(hwnd, IDC_MISSED_APPROACH_REMARKS, EM_SETLIMITTEXT, REMARKS_BUFFER - 1, NULL);
        return TRUE;
    }

    auto AcknowledgeMissedApproachDialog::SaveDialog(HWND hwnd) -> LRESULT
    {
        // Duration
        TCHAR buffer[REMARKS_BUFFER];
        SendDlgItemMessage(
            hwnd, MISSED_APPROACH_DRAW_DURATION, WM_GETTEXT, REMARKS_BUFFER, reinterpret_cast<LPARAM>(buffer));
        this->tagFunction->Acknowledge(HelperFunctions::ConvertToRegularString(buffer));

        return TRUE;
    }

    auto CALLBACK AcknowledgeMissedApproachDialog::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT
    {
        if (msg == WM_INITDIALOG) {
            LogInfo("Missed approach acknowledge dialog opened");
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<DialogCallArgument*>(lParam)->dialogArgument);
        } else if (msg == WM_DESTROY) {
            SetWindowLongPtr(hwnd, GWLP_USERDATA, NULL);
            LogInfo("Missed approach acknowledge dialog closed");
        }

        AcknowledgeMissedApproachDialog* dialog =
            reinterpret_cast<AcknowledgeMissedApproachDialog*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        return dialog != nullptr ? dialog->PrivateWindowProcedure(hwnd, msg, wParam, lParam) : FALSE;
    }

    auto AcknowledgeMissedApproachDialog::PrivateWindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        -> LRESULT
    {
        switch (msg) {
        // Window closed
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
            default:
                return FALSE;
            }
        }
        }

        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
} // namespace UKControllerPlugin::MissedApproach
