#include "pch/pch.h"

#include "dialog/DialogCallArgument.h"
#include "helper/HelperFunctions.h"
#include "minstack/MinStackConfigurationDialog.h"

using UKControllerPlugin::HelperFunctions;
using UKControllerPlugin::Dialog::DialogCallArgument;
using UKControllerPlugin::MinStack::MinStackManager;
using UKControllerPlugin::MinStack::MinStackRendererConfiguration;

namespace UKControllerPlugin::MinStack {

    MinStackConfigurationDialog::MinStackConfigurationDialog(const MinStackManager& manager) : manager(manager)
    {
    }

    /*
        Public facing window procedure
    */
    auto MinStackConfigurationDialog::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT
    {
        if (msg == WM_INITDIALOG) {
            LogInfo("Minstack configuration dialog opened");
            SetWindowLongPtr(
                hwnd,
                GWLP_USERDATA,
                reinterpret_cast<DialogCallArgument*>(lParam)->dialogArgument // NOLINT
            );
        } else if (msg == WM_DESTROY) {
            SetWindowLongPtr(hwnd, GWLP_USERDATA, NULL);
            LogInfo("Minstack configuration dialog closed");
        }

        MinStackConfigurationDialog* dialog = reinterpret_cast<MinStackConfigurationDialog*>( // NOLINT
            GetWindowLongPtr(hwnd, GWLP_USERDATA));
        return dialog != nullptr ? dialog->InternalWndProc(hwnd, msg, wParam, lParam) : FALSE;
    }

    /*
        Private window procedure bound to the objects
    */
    auto MinStackConfigurationDialog::InternalWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT
    {
        switch (msg) {
        // Initialise
        case WM_INITDIALOG: {
            this->InitDialog(hwnd, lParam);
            return TRUE;
        };
        // Dialog Closed
        case WM_CLOSE: {
            INT_PTR result = 0;
            EndDialog(hwnd, result);
            return TRUE;
        }
        // Buttons pressed
        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
            case IDOK: {
                this->SaveDialog(hwnd);
                INT_PTR result = 0;
                EndDialog(hwnd, result);
                return TRUE;
            }
            case IDCANCEL: {
                INT_PTR result = 0;
                EndDialog(hwnd, result);
                return TRUE;
            }
            case IDC_MINSTACK_ADD: {
                AddEntryToActiveList(hwnd);
                return TRUE;
            }
            case IDC_MINSTACK_DELETE: {
                RemoveEntryFromActiveList(hwnd);
                return TRUE;
            }
            case IDC_MINSTACK_DOWN: {
                this->SwapElements(hwnd, false);
                return TRUE;
            }
            case IDC_MINSTACK_UP: {
                this->SwapElements(hwnd, true);
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
    void MinStackConfigurationDialog::AddEntryToActiveList(HWND hwnd)
    {
        LPARAM selectedMinstackIndex = SendDlgItemMessage(hwnd, IDC_MINSTACK_SELECT, CB_GETCURSEL, NULL, NULL);

        // None selected, stop
        if (selectedMinstackIndex == LB_ERR) {
            return;
        }

        // Get the selected item data
        TCHAR bufferDisplayString[250]; // NOLINT

        HRESULT mslDisplayString = SendDlgItemMessage(
            hwnd,
            IDC_MINSTACK_SELECT,
            CB_GETLBTEXT,
            selectedMinstackIndex,
            reinterpret_cast<LPARAM>(bufferDisplayString) // NOLINT
        );

        LPARAM mslKeyData = SendDlgItemMessage(hwnd, IDC_MINSTACK_SELECT, CB_GETITEMDATA, selectedMinstackIndex, NULL);

        if (mslDisplayString == LB_ERR || mslKeyData == LB_ERR) {
            LogError("Failed to get MSL data from select");
            return;
        }

        // Get the insert position, insert
        LPARAM insertPosition = SendDlgItemMessage(hwnd, IDC_MINSTACK_LIST, LB_GETCURSEL, NULL, NULL);

        LPARAM insertedStringPosition = SendDlgItemMessage(
            hwnd,
            IDC_MINSTACK_LIST,
            LB_INSERTSTRING,
            insertPosition,
            reinterpret_cast<LPARAM>(bufferDisplayString) // NOLINT
        );

        if (insertedStringPosition == LB_ERR) {
            LogError("Failed to insert MSL into active list");
            return;
        }

        SendDlgItemMessage(hwnd, IDC_MINSTACK_LIST, LB_SETITEMDATA, insertedStringPosition, mslKeyData);

        // Remove from previous list
        SendDlgItemMessage(hwnd, IDC_MINSTACK_SELECT, CB_DELETESTRING, selectedMinstackIndex, NULL);
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
        LPARAM secondItemData)
    {
        // Delete the items
        SendDlgItemMessage(hwnd, IDC_MINSTACK_LIST, LB_DELETESTRING, secondItemIndex, NULL);

        SendDlgItemMessage(hwnd, IDC_MINSTACK_LIST, LB_DELETESTRING, firstItemIndex, NULL);

        // Put them back, in the other order
        SendDlgItemMessage(hwnd, IDC_MINSTACK_LIST, LB_INSERTSTRING, firstItemIndex, secondItemString);

        SendDlgItemMessage(hwnd, IDC_MINSTACK_LIST, LB_SETITEMDATA, firstItemIndex, secondItemData);

        SendDlgItemMessage(hwnd, IDC_MINSTACK_LIST, LB_INSERTSTRING, secondItemIndex, firstItemString);

        SendDlgItemMessage(hwnd, IDC_MINSTACK_LIST, LB_SETITEMDATA, secondItemIndex, firstItemData);
    }

    /*
        Initialise the controls from config
    */
    void MinStackConfigurationDialog::InitDialog(HWND hwnd, LPARAM lParam)
    {
        this->config = reinterpret_cast<MinStackRendererConfiguration*>(     // NOLINT
            reinterpret_cast<DialogCallArgument*>(lParam)->contextArgument); // NOLINT

        // Set the display checkbox
        CheckDlgButton(hwnd, IDC_MINSTACK_DISPLAY_CHECK, this->config->ShouldRender() ? BST_CHECKED : BST_UNCHECKED);

        // Add the selected MSLs to the existing list
        this->activeMslKeys = this->manager.GetAllMslKeys();
        for (const auto& minStackConfig : *this->config) {
            auto mslKey = this->activeMslKeys.find(minStackConfig.key);
            if (mslKey == this->activeMslKeys.cend()) {
                LogWarning("Unable to add Active MSL to list, not active: " + *mslKey);
                continue;
            }

            unsigned int itemIndex = SendDlgItemMessage(
                hwnd,
                IDC_MINSTACK_LIST,
                LB_ADDSTRING,
                NULL,
                reinterpret_cast<LPARAM>(this->GetListEntryForKey(*mslKey).c_str()) // NOLINT
            );

            SendDlgItemMessage(
                hwnd,
                IDC_MINSTACK_LIST,
                LB_SETITEMDATA,
                itemIndex,
                reinterpret_cast<LPARAM>((*mslKey).c_str()) // NOLINT
            );
        }

        // Add all non-active keys to the dropdown
        for (const auto& activeMslKey : this->activeMslKeys) {
            if (this->config->GetItem(activeMslKey) != this->config->InvalidItem()) {
                continue;
            }

            HRESULT itemIndex = SendDlgItemMessage(
                hwnd,
                IDC_MINSTACK_SELECT,
                CB_ADDSTRING,
                NULL,
                reinterpret_cast<LPARAM>(this->GetListEntryForKey(activeMslKey).c_str()) // NOLINT
            );

            SendDlgItemMessage(
                hwnd,
                IDC_MINSTACK_SELECT,
                CB_SETITEMDATA,
                itemIndex,
                reinterpret_cast<LPARAM>(activeMslKey.c_str()) // NOLINT
            );
        }
    }

    /*
        Swap two elements
    */
    void MinStackConfigurationDialog::SwapElements(HWND hwnd, bool swapUp)
    {
        LPARAM selectedMinstackIndex = SendDlgItemMessage(hwnd, IDC_MINSTACK_LIST, LB_GETCURSEL, NULL, NULL);

        LPARAM itemCount = SendDlgItemMessage(hwnd, IDC_MINSTACK_LIST, LB_GETCOUNT, NULL, NULL);

        // None selected, or at the extemities
        if (selectedMinstackIndex == LB_ERR || -itemCount == LB_ERR ||
            IsLastElement(selectedMinstackIndex, itemCount, swapUp)) {
            return;
        }

        unsigned int swapItemIndex = swapUp ? selectedMinstackIndex - 1 : selectedMinstackIndex + 1;

        // Get the selected item data
        TCHAR bufferDisplayStringOriginal[250]; // NOLINT
        HRESULT mslDisplayStringOriginal = SendDlgItemMessage(
            hwnd,
            IDC_MINSTACK_LIST,
            LB_GETTEXT,
            selectedMinstackIndex,
            reinterpret_cast<LPARAM>(bufferDisplayStringOriginal) // NOLINT
        );

        LPARAM mslKeyDataOriginal =
            SendDlgItemMessage(hwnd, IDC_MINSTACK_LIST, LB_GETITEMDATA, selectedMinstackIndex, NULL);

        if (mslDisplayStringOriginal == LB_ERR || mslKeyDataOriginal == LB_ERR) {
            LogError("Failed to get original item data for swap");
            return;
        }

        // Get the swap item data
        TCHAR bufferDisplayStringSwap[250]; // NOLINT
        HRESULT mslDisplayStringSwap = SendDlgItemMessage(
            hwnd,
            IDC_MINSTACK_LIST,
            LB_GETTEXT,
            swapItemIndex,
            reinterpret_cast<LPARAM>(bufferDisplayStringSwap) // NOLINT
        );

        LPARAM mslKeyDataSwap = SendDlgItemMessage(hwnd, IDC_MINSTACK_LIST, LB_GETITEMDATA, swapItemIndex, NULL);

        if (mslDisplayStringSwap == LB_ERR || mslKeyDataSwap == LB_ERR) {
            LogError("Failed to get original item data for swap");
            return;
        }

        // Do the Swap
        if (swapUp) {
            this->DoElementSwap(
                hwnd,
                swapItemIndex,
                reinterpret_cast<LPARAM>(bufferDisplayStringSwap), // NOLINT
                mslKeyDataSwap,
                selectedMinstackIndex,
                reinterpret_cast<LPARAM>(bufferDisplayStringOriginal), // NOLINT
                mslKeyDataOriginal);
        } else {
            this->DoElementSwap(
                hwnd,
                selectedMinstackIndex,
                reinterpret_cast<LPARAM>(bufferDisplayStringOriginal), // NOLINT
                mslKeyDataOriginal,
                swapItemIndex,
                reinterpret_cast<LPARAM>(bufferDisplayStringSwap), // NOLINT
                mslKeyDataSwap);
        }

        // Select the new item index
        SendDlgItemMessage(hwnd, IDC_MINSTACK_LIST, LB_SETCURSEL, swapItemIndex, NULL);
    }

    /*
        Remove entry from the active list
    */
    void MinStackConfigurationDialog::RemoveEntryFromActiveList(HWND hwnd)
    {
        LPARAM selectedMinstackIndex = SendDlgItemMessage(hwnd, IDC_MINSTACK_LIST, LB_GETCURSEL, NULL, NULL);

        // None selected, stop
        if (selectedMinstackIndex == LB_ERR) {
            return;
        }

        // Get the selected item data
        TCHAR bufferDisplayString[250]; // NOLINT
        HRESULT mslDisplayString = SendDlgItemMessage(
            hwnd,
            IDC_MINSTACK_LIST,
            LB_GETTEXT,
            selectedMinstackIndex,
            reinterpret_cast<LPARAM>(bufferDisplayString) // NOLINT
        );

        LPARAM mslKeyData = SendDlgItemMessage(hwnd, IDC_MINSTACK_LIST, LB_GETITEMDATA, selectedMinstackIndex, NULL);

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
            reinterpret_cast<LPARAM>(bufferDisplayString) // NOLINT
        );

        if (insertedStringPosition == LB_ERR) {
            LogError("Failed to insert MSL into selection list");
            return;
        }

        SendDlgItemMessage(hwnd, IDC_MINSTACK_SELECT, CB_SETITEMDATA, insertedStringPosition, mslKeyData);

        // Remove from previous list
        SendDlgItemMessage(hwnd, IDC_MINSTACK_LIST, LB_DELETESTRING, selectedMinstackIndex, NULL);
    }

    /*
        Save the controls to config
    */
    void MinStackConfigurationDialog::SaveDialog(HWND hwnd)
    {
        // Set display from check checkbox
        this->config->SetShouldRender(IsDlgButtonChecked(hwnd, IDC_MINSTACK_DISPLAY_CHECK) == BST_CHECKED);

        // Check how many items are in the minstack list
        int itemCount = SendDlgItemMessage(hwnd, IDC_MINSTACK_LIST, LB_GETCOUNT, NULL, NULL);

        if (itemCount == LB_ERR) {
            LogError("Unable to count items in MinStack list");
            return;
        }

        // Update the config from the list
        std::set<MinStackRenderedItem> newConfig;
        for (int i = 0; i < itemCount; i++) {
            LPARAM mslKey = SendDlgItemMessage(hwnd, IDC_MINSTACK_LIST, LB_GETITEMDATA, i, NULL);

            newConfig.insert({
                i,
                reinterpret_cast<const char*>(mslKey), // NOLINT
            });
        }

        this->config->Reset();
        for (const auto& newConfigItem : newConfig) {
            this->config->AddItem(newConfigItem);
        }
    }

    /*
        Transform each MSL key to how it should be displayed
    */
    auto MinStackConfigurationDialog::GetListEntryForKey(const std::string& mslKey) -> std::wstring
    {
        std::string facility = mslKey.substr(0, 3) == "tma" ? "TMA" : "Airfield";
        std::string identifier =
            facility == "TMA" ? mslKey.substr(TMA_SUBSTRING_LENGTH) : mslKey.substr(AIRFIELD_SUBSTRING_LENGTH);

        return HelperFunctions::ConvertToWideString(facility + " - " + identifier);
    }

    /*
        Is the selected element the last in the list?
    */
    auto MinStackConfigurationDialog::IsLastElement(unsigned int selectedIndex, unsigned int itemCount, bool swapUp)
        -> bool
    {
        return swapUp ? selectedIndex == 0 : selectedIndex == itemCount - 1;
    }
} // namespace UKControllerPlugin::MinStack
