#include "oceanic/OceanicClearanceDialog.h"
#include "dialog/DialogCallArgument.h"
#include "datablock/DatablockFunctions.h"
#include "oceanic/Clearance.h"
#include "helper/HelperFunctions.h"

using UKControllerPlugin::Datablock::ConvertAltitudeToFlightLevel;
using UKControllerPlugin::Dialog::DialogCallArgument;

namespace UKControllerPlugin {
    namespace Oceanic {

        /*
            Public facing window procedure
        */
        LRESULT OceanicClearanceDialog::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            if (msg == WM_INITDIALOG) {
                LogInfo("Oceanic clearance dialog opened");
                SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<DialogCallArgument*>(lParam)->dialogArgument);
            } else if (msg == WM_DESTROY) {
                SetWindowLongPtr(hwnd, GWLP_USERDATA, NULL);
                LogInfo("Oceanic clearance dialog closed");
            }

            OceanicClearanceDialog* dialog =
                reinterpret_cast<OceanicClearanceDialog*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            return dialog ? dialog->_WndProc(hwnd, msg, wParam, lParam) : FALSE;
        }

        /*
            Private window procedure bound to the objects
        */
        LRESULT OceanicClearanceDialog::_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
                }
            }
            }

            return FALSE;
        }

        void OceanicClearanceDialog::InitDialog(HWND hwnd, LPARAM lParam)
        {
            const Clearance* const clearance =
                reinterpret_cast<Clearance*>(reinterpret_cast<DialogCallArgument*>(lParam)->contextArgument);

            std::wstring callsignWide = this->ConvertItemForDialog(clearance->callsign);
            std::wstring titleText = std::wstring(L"Oceanic clearance for " + callsignWide);

            // Change the title bar
            SetWindowText(hwnd, titleText.c_str());

            // Set the static text items
            this->SetDialogItem(hwnd, IDC_OCEANIC_STATIC_CALLSIGN_DATA, callsignWide);
            this->SetDialogItem(hwnd, IDC_OCEANIC_STATIC_STATUS_DATA, ConvertItemForDialog(clearance->status));
            this->SetDialogItem(hwnd, IDC_OCEANIC_STATIC_TRACK_DATA, ConvertItemForDialog(clearance->track));
            this->SetDialogItem(hwnd, IDC_OCEANIC_STATIC_FIX_DATA, ConvertItemForDialog(clearance->entryFix));
            this->SetDialogItem(hwnd, IDC_OCEANIC_STATIC_LEVEL_DATA, ConvertItemForDialog(clearance->flightLevel));
            this->SetDialogItem(hwnd, IDC_OCEANIC_STATIC_MACH_DATA, ConvertItemForDialog(clearance->mach));
            this->SetDialogItem(hwnd, IDC_OCEANIC_STATIC_ENTRY_DATA, ConvertItemForDialog(clearance->entryTime));
            this->SetDialogItem(hwnd, IDC_OCEANIC_STATIC_ISSUED_DATA, ConvertItemForDialog(clearance->clearanceIssued));
            this->SetDialogItem(hwnd, IDC_OCEANIC_STATIC_EXTRA_DATA, ConvertItemForDialog(clearance->extra));
        }

        std::wstring OceanicClearanceDialog::ConvertItemForDialog(std::string item)
        {
            return item == "" ? L"N/A" : HelperFunctions::ConvertToWideString(item);
        }

        void OceanicClearanceDialog::SetDialogItem(HWND hwnd, int itemId, std::wstring value)
        {
            SendDlgItemMessage(hwnd, itemId, WM_SETTEXT, NULL, reinterpret_cast<LPARAM>(value.c_str()));
        }
    } // namespace Oceanic
} // namespace UKControllerPlugin
