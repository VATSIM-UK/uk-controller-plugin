#include "pch/stdafx.h"
#include "hold/HoldConfigurationDialog.h"
#include "hold/HoldDisplayFunctions.h"
#include "dialog/DialogCallArgument.h"

using UKControllerPlugin::Hold::ConvertToTchar;
using UKControllerPlugin::Dialog::DialogCallArgument;
using UKControllerPlugin::Hold::HoldConfigurationMenuItem;
using UKControllerPlugin::Navaids::NavaidCollection;

namespace UKControllerPlugin {
    namespace Hold {

        HoldConfigurationDialog::HoldConfigurationDialog(
            const NavaidCollection& navaids
        )
            : navaids(navaids)
        {

        }

        /*
            Initialise the dialog
        */
        void HoldConfigurationDialog::InitDialog(HWND hwnd, LPARAM lParam)
        {
            // One of the ES menu items spawned by the radar screen. If profiles are changed
            // this needs to be reported back to the menu item.
            this->configurationItem = reinterpret_cast<HoldConfigurationMenuItem *>(
                reinterpret_cast<DialogCallArgument *>(lParam)->contextArgument
            );

            HWND holdList = GetDlgItem(hwnd, IDC_HOLD_LIST);

            // Add checkboxes to the navaids list so they may be selected
            ListView_SetExtendedListViewStyle(holdList, LVS_EX_CHECKBOXES);


            LVITEM item;
            item.mask = LVIF_TEXT;
            item.iItem = 0;
            item.iSubItem = 0;
            // Load each navaid into the list and set its checked status
            for (
                NavaidCollection::const_iterator it = this->navaids.cbegin();
                it != this->navaids.cend();
                ++it
            ) {
                item.pszText = (LPWSTR)ConvertToTchar(it->identifier).c_str();
                ListView_InsertItem(
                    holdList,
                    &item
                );

                ListView_SetCheckState(holdList, item.iItem, true);
            }           
        }

        /*
            Destroy the dialog - clean up
        */
        void HoldConfigurationDialog::SaveDialog(HWND hwnd)
        {
            HWND holdList = GetDlgItem(hwnd, IDC_HOLD_LIST);

            int numItems = ListView_GetItemCount(holdList);
            for (int item = 0; item < numItems; ++item)
            {
                // Get the save status
            }
        }

        /*
            Private dialog procedure for the hold configuration dialog, should be used
            against a bound instance.
        */
        LRESULT HoldConfigurationDialog::_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
                        case IDC_HOLDS_SAVE: {
                            // OK clicked, close the window
                            this->SaveDialog(hwnd);
                            EndDialog(hwnd, wParam);
                            return TRUE;
                        }
                    }
                }
            }
            return FALSE;
        }

        /*
            Public dialog procedure for the hold configuration dialog.
        */
        LRESULT HoldConfigurationDialog::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            if (msg == WM_INITDIALOG) {
                LogInfo("Hold configuration dialog opened");
                SetWindowLongPtr(
                    hwnd,
                    GWLP_USERDATA,
                    reinterpret_cast<DialogCallArgument *>(lParam)->dialogArgument
                );
            } else if (msg == WM_DESTROY) {
                SetWindowLongPtr(hwnd, GWLP_USERDATA, NULL);
                LogInfo("Hold configuration dialog closed");
            }

            HoldConfigurationDialog * dialog = reinterpret_cast<HoldConfigurationDialog*>(
                GetWindowLongPtr(hwnd, GWLP_USERDATA)
            );
            return dialog ? dialog->_WndProc(hwnd, msg, wParam, lParam) : FALSE;
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
