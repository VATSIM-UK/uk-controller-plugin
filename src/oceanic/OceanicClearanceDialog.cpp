#include "pch/stdafx.h"
#include "oceanic/OceanicClearanceDialog.h"
#include "dialog/DialogCallArgument.h"
#include "hold/HoldDisplayFunctions.h"
#include "helper/HelperFunctions.h"
#include "api/ApiException.h"
#include "datablock/DatablockFunctions.h"

using UKControllerPlugin::Dialog::DialogCallArgument;
using UKControllerPlugin::Api::ApiException;
using UKControllerPlugin::Datablock::ConvertAltitudeToFlightLevel;

namespace UKControllerPlugin {
    namespace Oceanic {

        /*
            Public facing window procedure
        */
        LRESULT OceanicClearanceDialog::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            if (msg == WM_INITDIALOG) {
                LogInfo("Oceanic clearance dialog opened");
                SetWindowLongPtr(
                    hwnd,
                    GWLP_USERDATA,
                    reinterpret_cast<DialogCallArgument*>(lParam)->dialogArgument
                );
            } else if (msg == WM_DESTROY) {
                SetWindowLongPtr(hwnd, GWLP_USERDATA, NULL);
                LogInfo("Oceanic clearance dialog closed");
            }

            OceanicClearanceDialog* dialog = reinterpret_cast<OceanicClearanceDialog*>(
                GetWindowLongPtr(hwnd, GWLP_USERDATA)
            );
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
        { }
    } // namespace Oceanic
}  // namespace UKControllerPlugin
