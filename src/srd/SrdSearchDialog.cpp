#include "pch/stdafx.h"
#include "srd/SrdSearchDialog.h"
#include "dialog/DialogCallArgument.h"

using UKControllerPlugin::Dialog::DialogCallArgument;

namespace UKControllerPlugin {
    namespace Srd {

        /*
            Public facing window procedure
        */
        LRESULT SrdSearchDialog::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            if (msg == WM_INITDIALOG) {
                LogInfo("SRD search dialog opened");
                SetWindowLongPtr(
                    hwnd,
                    GWLP_USERDATA,
                    reinterpret_cast<DialogCallArgument*>(lParam)->dialogArgument
                );
            }
            else if (msg == WM_DESTROY) {
                SetWindowLongPtr(hwnd, GWLP_USERDATA, NULL);
                LogInfo("Minstack configuration dialog closed");
            }

            SrdSearchDialog* dialog = reinterpret_cast<SrdSearchDialog*>(
                GetWindowLongPtr(hwnd, GWLP_USERDATA)
            );
            return dialog ? dialog->_WndProc(hwnd, msg, wParam, lParam) : FALSE;
        }

        /*
            Private window procedure bound to the objects
        */
        LRESULT SrdSearchDialog::_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            switch (msg) {
                // Initialise
                case WM_INITDIALOG: {
                    this->InitDialog(hwnd, lParam);
                    return TRUE;
                };
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
                        case IDCANCEL: {
                            EndDialog(hwnd, wParam);
                            return TRUE;
                        }
                    }
                }
            }

            return FALSE;
        }

        void SrdSearchDialog::InitDialog(HWND hwnd, LPARAM lParam)
        {
            HWND resultsList = GetDlgItem(hwnd, IDC_SRD_RESULTS);

            // Create the min level column
            LVCOLUMN minLevelColumn = {
                LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM,
                LVCFMT_LEFT | LVCFMT_FIXED_WIDTH,
                50,
                L"Min",
                0
            };
            ListView_InsertColumn(resultsList, 0, &minLevelColumn);

            // Create the max level column
            LVCOLUMN maxLevelColumn = {
                LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM,
                LVCFMT_LEFT | LVCFMT_FIXED_WIDTH,
                50,
                L"Max",
                1
            };
            ListView_InsertColumn(resultsList, 1, &maxLevelColumn);

            // Create the route string column
            LVCOLUMN routeStringColumn = {
                LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM,
                LVCFMT_LEFT | LVCFMT_FIXED_WIDTH,
                335,
                L"Route String",
                2
            };
            ListView_InsertColumn(resultsList, 2, &routeStringColumn);
        }

    }  // namespace Srd
}  // UKControllerPlugin
