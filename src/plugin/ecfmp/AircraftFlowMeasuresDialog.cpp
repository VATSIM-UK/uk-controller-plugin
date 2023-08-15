#include "AircraftFlowMeasuresDialog.h"
#include "dialog/DialogCallArgument.h"

namespace UKControllerPlugin::ECFMP {
    /*
        Public facing window procedure
    */
    LRESULT AircraftFlowMeasuresDialog::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        if (msg == WM_INITDIALOG) {
            LogInfo("Aircraft flow measures dialog opened");
            SetWindowLongPtr(
                hwnd, GWLP_USERDATA, reinterpret_cast<Dialog::DialogCallArgument*>(lParam)->dialogArgument);
        } else if (msg == WM_DESTROY) {
            SetWindowLongPtr(hwnd, GWLP_USERDATA, NULL);
            LogInfo("Aircraft flow measures dialog closed");
        }

        AircraftFlowMeasuresDialog* dialog =
            reinterpret_cast<AircraftFlowMeasuresDialog*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        return dialog ? dialog->_WndProc(hwnd, msg, wParam, lParam) : FALSE;
    }

    /*
        Private window procedure bound to the objects
    */
    LRESULT AircraftFlowMeasuresDialog::_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        switch (msg) {
        // Initialise
        case WM_INITDIALOG: {
            this->InitDialog(hwnd, lParam);
            return TRUE;
        }
        // Dialog Closed
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

    void AircraftFlowMeasuresDialog::InitDialog(HWND hwnd, LPARAM lParam)
    {
        auto* data = reinterpret_cast<AircraftFlowMeasuresDialogData*>(
            reinterpret_cast<Dialog::DialogCallArgument*>(lParam)->contextArgument);

        // Set the the dialog box title
        std::wstring title = L"ECFMP flow Measures for " + data->callsign;
        SetWindowText(hwnd, title.c_str());

        // Set the flow measures text
        std::wstring description = data->flowMeasures;
        SetDlgItemText(hwnd, IDC_FLOW_MEASURES_INFO, description.c_str());
    }
} // namespace UKControllerPlugin::ECFMP
