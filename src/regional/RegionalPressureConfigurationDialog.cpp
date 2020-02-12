#include "pch/stdafx.h"
#include "regional/RegionalPressureConfigurationDialog.h"
#include "dialog/DialogCallArgument.h"
#include "helper/HelperFunctions.h"

using UKControllerPlugin::Dialog::DialogCallArgument;
using UKControllerPlugin::Regional::RegionalPressureConfigurationDialog;
using UKControllerPlugin::Regional::RegionalPressureConfigurationDialog;
using UKControllerPlugin::HelperFunctions;

namespace UKControllerPlugin {
    namespace Regional {

        RegionalPressureConfigurationDialog::RegionalPressureConfigurationDialog(const RegionalPressureManager & manager)
            : manager(manager)
        {

        }

        /*
            Public facing window procedure
        */
        LRESULT RegionalPressureConfigurationDialog::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            if (msg == WM_INITDIALOG) {
                LogInfo("Regional pressure configuration dialog opened");
                SetWindowLongPtr(
                    hwnd,
                    GWLP_USERDATA,
                    reinterpret_cast<DialogCallArgument *>(lParam)->dialogArgument
                );
            } else if (msg == WM_DESTROY) {
                SetWindowLongPtr(hwnd, GWLP_USERDATA, NULL);
                LogInfo("Regional pressure configuration dialog closed");
            }

            RegionalPressureConfigurationDialog * dialog = reinterpret_cast<RegionalPressureConfigurationDialog*>(
                GetWindowLongPtr(hwnd, GWLP_USERDATA)
            );
            return dialog ? dialog->_WndProc(hwnd, msg, wParam, lParam) : FALSE;
        }

        /*
            Private window procedure bound to the objects
        */
        LRESULT RegionalPressureConfigurationDialog::_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
                            this->SaveDialog(hwnd);
                            EndDialog(hwnd, wParam);
                            return TRUE;
                        }
                        case IDCANCEL: {
                            EndDialog(hwnd, wParam);
                            return TRUE;
                        }
                        case IDC_MINSTACK_ADD: {
                            this->AddEntryToActiveList(hwnd, wParam);
                            return TRUE;
                        }
                        case IDC_MINSTACK_DELETE: {
                            this->RemoveEntryFromActiveList(hwnd, wParam);
                            return TRUE;
                        }
                        case IDC_MINSTACK_DOWN: {
                            this->SwapElements(hwnd, lParam, false);
                            return TRUE;
                        }
                        case IDC_MINSTACK_UP: {
                            this->SwapElements(hwnd, lParam, true);
                            return TRUE;
                        }
                    }
                }
            }

            return FALSE;
        }

        /*
            Take an item from the selection and add it to the active list
        */
        void RegionalPressureConfigurationDialog::AddEntryToActiveList(HWND hwnd, LPARAM lParam)
        {
            LPARAM selectedMinstackIndex = SendDlgItemMessage(
                hwnd,
                IDC_MINSTACK_SELECT,
                CB_GETCURSEL,
                NULL,
                NULL
            );

            // None selected, stop
            if (selectedMinstackIndex == LB_ERR) {
                return;
            }

            // Get the selected item data
            TCHAR bufferDisplayString[250];

            HRESULT mslDisplayString = SendDlgItemMessage(
                hwnd,
                IDC_MINSTACK_SELECT,
                CB_GETLBTEXT,
                selectedMinstackIndex,
                reinterpret_cast<LPARAM>(bufferDisplayString)
            );

            LPARAM mslKeyData = SendDlgItemMessage(
                hwnd,
                IDC_MINSTACK_SELECT,
                CB_GETITEMDATA,
                selectedMinstackIndex,
                NULL
            );

            if (mslDisplayString == LB_ERR || mslKeyData == LB_ERR) {
                LogError("Failed to get MSL data from select");
                return;
            }

            // Get the insert position, insert
            LPARAM insertPosition = SendDlgItemMessage(
                hwnd,
                IDC_MINSTACK_LIST,
                LB_GETCURSEL,
                NULL,
                NULL
            );

            LPARAM insertedStringPosition = SendDlgItemMessage(
                hwnd,
                IDC_MINSTACK_LIST,
                LB_INSERTSTRING,
                insertPosition,
                reinterpret_cast<LPARAM>(bufferDisplayString)
            );

            if (insertedStringPosition == LB_ERR) {
                LogError("Failed to insert MSL into active list");
                return;
            }

            LPARAM insertedData = SendDlgItemMessage(
                hwnd,
                IDC_MINSTACK_LIST,
                LB_SETITEMDATA,
                insertedStringPosition,
                mslKeyData
            );

            // Remove from previous list
            SendDlgItemMessage(
                hwnd,
                IDC_MINSTACK_SELECT,
                CB_DELETESTRING,
                selectedMinstackIndex,
                NULL
            );
        }

        /*
            Swap two items in the list box, the second item should be AFTER the first in
            the order.
        */
        void MinStackConfigurationDialog::DoElementSwap(
            HWND hwnd,
            unsigned int firstItemIndex,
            LPARAM firstItemString,
            LPARAM firstItemData,
            unsigned int secondItemIndex,
            LPARAM secondItemString,
            LPARAM secondItemData
        ) {
            // Delete the items
            SendDlgItemMessage(
                hwnd,
                IDC_MINSTACK_LIST,
                LB_DELETESTRING,
                secondItemIndex,
                NULL
            );

            SendDlgItemMessage(
                hwnd,
                IDC_MINSTACK_LIST,
                LB_DELETESTRING,
                firstItemIndex,
                NULL
            );

            // Put them back, in the other order
            SendDlgItemMessage(
                hwnd,
                IDC_MINSTACK_LIST,
                LB_INSERTSTRING,
                firstItemIndex,
                secondItemString
            );

            SendDlgItemMessage(
                hwnd,
                IDC_MINSTACK_LIST,
                LB_SETITEMDATA,
                firstItemIndex,
                secondItemData
            );

            SendDlgItemMessage(
                hwnd,
                IDC_MINSTACK_LIST,
                LB_INSERTSTRING,
                secondItemIndex,
                firstItemString
            );

            SendDlgItemMessage(
                hwnd,
                IDC_MINSTACK_LIST,
                LB_SETITEMDATA,
                secondItemIndex,
                firstItemData
            );
        }

        /*
            Initialise the controls from config
        */
        void MinStackConfigurationDialog::InitDialog(HWND hwnd, LPARAM lParam)
        {
            this->config = reinterpret_cast<MinStackRendererConfiguration *>(
                reinterpret_cast<DialogCallArgument *>(lParam)->contextArgument
            );

            // Set the display checkbox
            CheckDlgButton(
                hwnd,
                IDC_MINSTACK_DISPLAY_CHECK,
                this->config->ShouldRender() ? BST_CHECKED : BST_UNCHECKED
            );

            // Add the selected MSLs to the existing list
            this->activeMslKeys = this->manager.GetAllMslKeys();
            for (
                MinStackRendererConfiguration::const_iterator it = this->config->cbegin();
                it != this->config->cend();
                ++it
            ) {
                auto mslKey = this->activeMslKeys.find(it->key);
                if (mslKey == this->activeMslKeys.cend()) {
                    LogWarning("Unable to add Active MSL to list, not active: " + *mslKey);
                    continue;
                }

                unsigned int itemIndex = SendDlgItemMessage(
                    hwnd,
                    IDC_MINSTACK_LIST,
                    LB_ADDSTRING,
                    NULL,
                    reinterpret_cast<LPARAM>(this->GetListEntryForKey(*mslKey).c_str())
                );

                SendDlgItemMessage(
                    hwnd,
                    IDC_MINSTACK_LIST,
                    LB_SETITEMDATA,
                    itemIndex,
                    reinterpret_cast<LPARAM>((*mslKey).c_str())
                );
            }

            // Add all non-active keys to the dropdown
            for (
                std::set<std::string>::const_iterator it = this->activeMslKeys.cbegin();
                it != this->activeMslKeys.cend();
                ++it
            ) {
                if (this->config->GetItem(*it) != this->config->invalidItem) {
                    continue;
                }

                HRESULT itemIndex = SendDlgItemMessage(
                    hwnd,
                    IDC_MINSTACK_SELECT,
                    CB_ADDSTRING,
                    NULL,
                    reinterpret_cast<LPARAM>(this->GetListEntryForKey(*it).c_str())
                );

                SendDlgItemMessage(
                    hwnd,
                    IDC_MINSTACK_SELECT,
                    CB_SETITEMDATA,
                    itemIndex,
                    reinterpret_cast<LPARAM>(it->c_str())
                );
            }
        }

        /*
            Swap two elements
        */
        void RegionalPressureConfigurationDialog::SwapElements(HWND hwnd, LPARAM lParam, bool swapUp)
        {
            LPARAM selectedMinstackIndex = SendDlgItemMessage(
                hwnd,
                IDC_MINSTACK_LIST,
                LB_GETCURSEL,
                NULL,
                NULL
            );

            LPARAM itemCount = SendDlgItemMessage(
                hwnd,
                IDC_MINSTACK_LIST,
                LB_GETCOUNT,
                NULL,
                NULL
            );

            // None selected, or at the extemities
            if (
                selectedMinstackIndex == LB_ERR ||
-                itemCount == LB_ERR ||
                this->IsLastElement(selectedMinstackIndex, itemCount, swapUp)
            ) {
                return;
            }

            unsigned int swapItemIndex = swapUp ? selectedMinstackIndex - 1 : selectedMinstackIndex + 1;

            // Get the selected item data
            TCHAR bufferDisplayStringOriginal[250];
            HRESULT mslDisplayStringOriginal = SendDlgItemMessage(
                hwnd,
                IDC_MINSTACK_LIST,
                LB_GETTEXT,
                selectedMinstackIndex,
                reinterpret_cast<LPARAM>(bufferDisplayStringOriginal)
            );

            LPARAM mslKeyDataOriginal = SendDlgItemMessage(
                hwnd,
                IDC_MINSTACK_LIST,
                LB_GETITEMDATA,
                selectedMinstackIndex,
                NULL
            );

            if (mslDisplayStringOriginal == LB_ERR || mslKeyDataOriginal == LB_ERR) {
                LogError("Failed to get original item data for swap");
                return;
            }

            // Get the swap item data
            TCHAR bufferDisplayStringSwap[250];
            HRESULT mslDisplayStringSwap = SendDlgItemMessage(
                hwnd,
                IDC_MINSTACK_LIST,
                LB_GETTEXT,
                swapItemIndex,
                reinterpret_cast<LPARAM>(bufferDisplayStringSwap)
            );

            LPARAM mslKeyDataSwap = SendDlgItemMessage(
                hwnd,
                IDC_MINSTACK_LIST,
                LB_GETITEMDATA,
                swapItemIndex,
                NULL
            );

            if (mslDisplayStringSwap == LB_ERR || mslKeyDataSwap == LB_ERR) {
                LogError("Failed to get original item data for swap");
                return;
            }

            // Do the Swap
            if (swapUp) {
                this->DoElementSwap(
                    hwnd,
                    swapItemIndex,
                    reinterpret_cast<LPARAM>(bufferDisplayStringSwap),
                    mslKeyDataSwap,
                    selectedMinstackIndex,
                    reinterpret_cast<LPARAM>(bufferDisplayStringOriginal),
                    mslKeyDataOriginal
                );
            } else {
                this->DoElementSwap(
                    hwnd,
                    selectedMinstackIndex,
                    reinterpret_cast<LPARAM>(bufferDisplayStringOriginal),
                    mslKeyDataOriginal,
                    swapItemIndex,
                    reinterpret_cast<LPARAM>(bufferDisplayStringSwap),
                    mslKeyDataSwap
                );
            }

            // Select the new item index
            SendDlgItemMessage(
                hwnd,
                IDC_MINSTACK_LIST,
                LB_SETCURSEL,
                swapItemIndex,
                NULL
            );
        }

        /*
            Remove entry from the active list
        */
        void MinStackConfigurationDialog::RemoveEntryFromActiveList(HWND hwnd, LPARAM lParam)
        {
            LPARAM selectedMinstackIndex = SendDlgItemMessage(
                hwnd,
                IDC_MINSTACK_LIST,
                LB_GETCURSEL,
                NULL,
                NULL
            );

            // None selected, stop
            if (selectedMinstackIndex == LB_ERR) {
                return;
            }

            // Get the selected item data
            TCHAR bufferDisplayString[250];
            HRESULT mslDisplayString = SendDlgItemMessage(
                hwnd,
                IDC_MINSTACK_LIST,
                LB_GETTEXT,
                selectedMinstackIndex,
                reinterpret_cast<LPARAM>(bufferDisplayString)
            );

            LPARAM mslKeyData = SendDlgItemMessage(
                hwnd,
                IDC_MINSTACK_LIST,
                LB_GETITEMDATA,
                selectedMinstackIndex,
                NULL
            );

            if (mslDisplayString == LB_ERR || mslKeyData == LB_ERR) {
                LogError("Failed to get MSL data from active list");
                return;
            }

            // Insert into the selection list
            LPARAM insertedStringPosition = SendDlgItemMessage(
                hwnd,
                IDC_MINSTACK_SELECT,
                CB_ADDSTRING,
                NULL,
                reinterpret_cast<LPARAM>(bufferDisplayString)
            );

            if (insertedStringPosition == LB_ERR) {
                LogError("Failed to insert MSL into selection list");
                return;
            }

            LPARAM insertedData = SendDlgItemMessage(
                hwnd,
                IDC_MINSTACK_SELECT,
                CB_SETITEMDATA,
                insertedStringPosition,
                mslKeyData
            );

            // Remove from previous list
            SendDlgItemMessage(
                hwnd,
                IDC_MINSTACK_LIST,
                LB_DELETESTRING,
                selectedMinstackIndex,
                NULL
            );
        }

        /*
            Save the controls to config
        */
        void MinStackConfigurationDialog::SaveDialog(HWND hwnd)
        {
            // Set display from check checkbox
            this->config->SetShouldRender(IsDlgButtonChecked(hwnd, IDC_MINSTACK_DISPLAY_CHECK) == BST_CHECKED);

            // Check how many items are in the minstack list
            unsigned int itemCount = SendDlgItemMessage(
                hwnd,
                IDC_MINSTACK_LIST,
                LB_GETCOUNT,
                NULL,
                NULL
            );

            if (itemCount == LB_ERR) {
                LogError("Unable to count items in MinStack list");
                return;
            }

            // Update the config from the list
            std::set<MinStackRenderedItem> newConfig;
            for (unsigned int i = 0; i < itemCount; i++) {
                LPARAM mslKey = SendDlgItemMessage(
                    hwnd,
                    IDC_MINSTACK_LIST,
                    LB_GETITEMDATA,
                    i,
                    NULL
                );

                newConfig.insert(
                    {
                        i,
                        reinterpret_cast<const char *>(mslKey),
                    }
                );
            }

           this->config->Reset();
           for (
               std::set<MinStackRenderedItem>::const_iterator it = newConfig.cbegin();
               it != newConfig.cend();
               ++it
            ) {
               this->config->AddItem(*it);
           }
        }

        /*
            Transform each MSL key to how it should be displayed
        */
        std::wstring RegionalPressureConfigurationDialog::GetListEntryForKey(std::string mslKey)
        {
            std::string facility = mslKey.substr(0, 3) == "tma" ? "TMA" : "Airfield";
            std::string identifier = facility == "TMA" ? mslKey.substr(4) : mslKey.substr(9);

            return HelperFunctions::ConvertToWideString(facility + " - " + identifier);
        }

        /*
            Is the selected element the last in the list?
        */
        bool RegionalPressureConfigurationDialog::IsLastElement(unsigned int selectedIndex, unsigned int itemCount, bool swapUp)
        {
            return swapUp ? selectedIndex == 0 : selectedIndex == itemCount - 1;
        }
    }  // namespace Regional
}  // namespace UKControllerPlugin
