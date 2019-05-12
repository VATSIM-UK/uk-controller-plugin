#include "pch/stdafx.h"
#include "hold/HoldConfigurationDialog.h"
#include "hold/HoldDisplayFunctions.h"
#include "hold/HoldProfileManager.h"
#include "dialog/DialogCallArgument.h"

using UKControllerPlugin::Hold::ConvertToTchar;
using UKControllerPlugin::Hold::GetSelectedHoldProfileText;
using UKControllerPlugin::Hold::HoldProfileManager;
using UKControllerPlugin::Dialog::DialogCallArgument;
using UKControllerPlugin::Hold::HoldConfigurationMenuItem;

namespace UKControllerPlugin {
    namespace Hold {

        HoldConfigurationDialog::HoldConfigurationDialog(
            HoldProfileManager & holdProfileManager
        )
            : holdProfileManager(holdProfileManager)
        {

        }

        /*
            Add a hold to the dialog.
        */
        bool HoldConfigurationDialog::AddHold(UKControllerPlugin::Hold::HoldingData hold)
        {
            if (!this->holds.insert(std::move(hold)).second) {
                LogWarning("Attempted to add duplicate hold to dialog " + std::to_string(hold.identifier));
                return false;
            }

            return true;
        }

        /*
            Return the number of holds.
        */
        size_t HoldConfigurationDialog::CountHolds(void) const
        {
            return this->holds.size();
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

            // Load the holds into the hold selector.
            SendDlgItemMessage(
                hwnd,
                IDC_HOLD_SELECTOR,
                CB_SETMINVISIBLE,
                10,
                0
            );

            for (
                std::set<HoldingData>::const_iterator it = this->holds.cbegin();
                it != this->holds.cend();
                ++it
                ) {
                int index = SendDlgItemMessage(
                    hwnd,
                    IDC_HOLD_SELECTOR,
                    CB_ADDSTRING,
                    0,
                    reinterpret_cast<LPARAM>(ConvertToTchar(it->description).c_str())
                );
                SendDlgItemMessage(
                    hwnd,
                    IDC_HOLD_SELECTOR,
                    CB_SETITEMDATA,
                    index,
                    (LPARAM)it->identifier
                );
            }

            SendDlgItemMessage(
                hwnd,
                IDC_HOLD_SELECTOR,
                CB_SETCURSEL,
                0,
                0
            );

            // Load the hold profiles into the profile selector
            if (this->holdProfileManager.CountProfiles() == 0) {
                return;
            }

            for (
                HoldProfileManager::HoldProfiles::const_iterator it = this->holdProfileManager.cbegin();
                it != this->holdProfileManager.cend();
                ++it
            ) {
                int index = SendDlgItemMessage(
                    hwnd,
                    IDC_HOLD_PROFILE_LIST,
                    LB_ADDSTRING,
                    NULL,
                    reinterpret_cast<LPARAM>(ConvertToTchar(it->name).c_str())
                );
                SendDlgItemMessage(
                    hwnd,
                    IDC_HOLD_PROFILE_LIST,
                    LB_SETITEMDATA,
                    index,
                    (LPARAM)it->id
                );
            }

            // Set the first hold in the list to be automatically selected and text limit on edit control
            SendDlgItemMessage(
                hwnd,
                IDC_HOLD_SELECTOR,
                LB_SETCURSEL,
                0,
                0
            );
            SendDlgItemMessage(
                hwnd,
                IDC_HOLD_PROFILE_NAME_EDIT,
                EM_SETLIMITTEXT,
                255,
                0
            );
        }

        /*
            Remove the currently selected hold from the list
            of those to be attributed to a profile
        */
        void HoldConfigurationDialog::RemoveHoldFromSelectedList(HWND hwnd)
        {
            const int selectedIndex = SendDlgItemMessage(
                hwnd,
                IDC_HOLD_LIST,
                LB_GETCURSEL,
                0,
                0
            );

            if (selectedIndex == LB_ERR) {
                // Nothing selected
                return;
            }

            const int selectedHoldId = SendDlgItemMessage(
                hwnd,
                IDC_HOLD_LIST,
                LB_GETITEMDATA,
                selectedIndex,
                0
            );

            SendDlgItemMessage(
                hwnd,
                IDC_HOLD_LIST,
                LB_DELETESTRING,
                selectedIndex,
                NULL
            );

            this->selectedHolds.erase(selectedHoldId);
        }

        /*
            Add the currently selected hold to the list of those selected
            for the profile
        */
        void HoldConfigurationDialog::AddHoldToSelectedList(HWND hwnd)
        {
            const int selectedIndex = SendDlgItemMessage(
                hwnd,
                IDC_HOLD_SELECTOR,
                CB_GETCURSEL,
                0,
                0
            );

            if (selectedIndex == LB_ERR) {
                // They've selected the prompt text, dont carry on
                return;
            }

            const int selectedHoldId = SendDlgItemMessage(
                hwnd,
                IDC_HOLD_SELECTOR,
                CB_GETITEMDATA,
                selectedIndex,
                0
            );

            // Make sure the holds are there
            auto hold = this->holds.find(selectedHoldId);
            if (hold == this->holds.cend()) {
                return;
            }

            if (!this->selectedHolds.insert(selectedHoldId).second) {
                return;
            }

            // Add the string to the box and attribute it to a hold
            int insertedIndex = SendDlgItemMessage(
                hwnd,
                IDC_HOLD_LIST,
                LB_ADDSTRING,
                NULL,
                reinterpret_cast<LPARAM>(ConvertToTchar(hold->description).c_str())
            );
            SendDlgItemMessage(
                hwnd,
                IDC_HOLD_LIST,
                LB_SETITEMDATA,
                insertedIndex,
                (LPARAM)selectedHoldId
            );
        }

        /*
            Using the selected holds and profile name create a new profile.
        */
        void HoldConfigurationDialog::CreateNewHoldProfile(HWND hwnd)
        {
            // Creating a new hold profile
            TCHAR buffer[255];

            // Get the text out of the profile box
            SendDlgItemMessage(
                hwnd,
                IDC_HOLD_PROFILE_NAME_EDIT,
                WM_GETTEXT,
                255,
                reinterpret_cast<LPARAM>(&buffer)
            );
            std::string profileName = ConvertFromTchar(buffer);

            // Get the holds.
            if (this->selectedHolds.size() == 0) {
                return;
            }

            // Save the new profile and add to lists
            int profileId = this->holdProfileManager.SaveNewProfile(profileName, this->selectedHolds);
            if (profileId == this->holdProfileManager.invalidProfileNewId) {
                return;
            }

            int newProfileIndex = SendDlgItemMessage(
                hwnd,
                IDC_HOLD_PROFILE_LIST,
                LB_ADDSTRING,
                NULL,
                reinterpret_cast<LPARAM>(buffer)
            );
            SendDlgItemMessage(
                hwnd,
                IDC_HOLD_PROFILE_LIST,
                LB_SETITEMDATA,
                newProfileIndex,
                (LPARAM)profileId
            );

            // Set the current selection to the new profile
            SendDlgItemMessage(
                hwnd,
                IDC_HOLD_PROFILE_LIST,
                LB_SETCURSEL,
                newProfileIndex,
                NULL
            );
            this->selectedHoldProfile = profileId;
            this->selectedHoldProfileIndex = newProfileIndex;
        }

        /*
            Delete the selected hold profile.
        */
        void HoldConfigurationDialog::DeleteHoldProfile(HWND hwnd)
        {
            // They want to delete a hold profile
            const int selectedIndex = SendDlgItemMessage(
                hwnd,
                IDC_HOLD_PROFILE_LIST,
                LB_GETCURSEL,
                NULL,
                NULL
            );

            // Cant delete an invalid entry
            if (selectedIndex == LB_ERR) {
                return;
            }

            const int profileId = SendDlgItemMessage(
                hwnd,
                IDC_HOLD_PROFILE_LIST,
                LB_GETITEMDATA,
                selectedIndex,
                NULL
            );

            if (!this->holdProfileManager.DeleteProfile(profileId)) {
                return;
            }

            // Remove from the combobox and reset selection
            SendDlgItemMessage(
                hwnd,
                IDC_HOLD_PROFILE_LIST,
                LB_DELETESTRING,
                selectedIndex,
                NULL
            );

            this->selectedHoldProfile = 0;
            this->selectedHoldProfileIndex = LB_ERR;
        }

        /*
            Destroy the dialog - clean up
        */
        void HoldConfigurationDialog::DestroyDialog(HWND hwnd)
        {
            this->selectedHolds.clear();
            this->selectedHoldProfile = 0;
            this->selectedHoldProfileIndex = LB_ERR;
        }

        /*
            Tell the radar screen that triggered this dialog to open the selected hold
            profile of holds.
        */
        void HoldConfigurationDialog::DisplaySelectedProfile()
        {
            this->configurationItem->SelectProfile(this->selectedHoldProfile);
        }

        /*
            Select a profile to modify from the dropdown
        */
        void HoldConfigurationDialog::SelectProfile(HWND hwnd, WPARAM wParam)
        {
            // If it's not the item changing, nothing to do.
            if (HIWORD(wParam) != LBN_SELCHANGE) {
                return;
            }

            // Change the profile selection index
            int selectedIndex = SendDlgItemMessage(
                hwnd,
                IDC_HOLD_PROFILE_LIST,
                LB_GETCURSEL,
                NULL,
                NULL
            );
            int selectedProfileId = SendDlgItemMessage(
                hwnd,
                IDC_HOLD_PROFILE_LIST,
                LB_GETITEMDATA,
                selectedIndex,
                NULL
            );
            this->selectedHoldProfile = selectedProfileId;
            this->selectedHoldProfileIndex = selectedIndex;

            // Put the holds from the selected profile in the list
            SendDlgItemMessage(
                hwnd,
                IDC_HOLD_LIST,
                LB_RESETCONTENT,
                this->selectedHoldProfile,
                NULL
            );

            this->selectedHolds.clear();
            HoldProfile profile = this->holdProfileManager.GetProfile(this->selectedHoldProfile);
            for (
                std::set<unsigned int>::const_iterator holdIt = profile.holds.cbegin();
                holdIt != profile.holds.cend();
                ++holdIt
            ) {
                auto hold = this->holds.find(*holdIt);
                if (hold == this->holds.cend()) {
                    LogWarning("Profile contained invalid hold " + std::to_string(*holdIt));
                    continue;
                }

                this->selectedHolds.insert(*holdIt);
                int index = SendDlgItemMessage(
                    hwnd,
                    IDC_HOLD_LIST,
                    LB_INSERTSTRING,
                    NULL,
                    reinterpret_cast<LPARAM>(ConvertToTchar(hold->description).c_str())
                );
                SendDlgItemMessage(
                    hwnd,
                    IDC_HOLD_LIST,
                    LB_SETITEMDATA,
                    index,
                    (LPARAM)hold->identifier
                );
            }

            // Put the hold profile name in the name edit box
            SendDlgItemMessage(
                hwnd,
                IDC_HOLD_PROFILE_NAME_EDIT,
                WM_SETTEXT,
                NULL,
                reinterpret_cast<LPARAM>(ConvertToTchar(profile.name).c_str())
            );
        }

        /*
            Update a hold profile based on the selected name and items.
        */
        void HoldConfigurationDialog::UpdateHoldProfile(HWND hwnd)
        {
            // If no hold profile selected, stop
            if (this->selectedHoldProfileIndex == LB_ERR) {
                return;
            }

            // No holds selected
            if (this->selectedHolds.size() == 0) {
                return;
            }

            // Get the new profile name
            TCHAR buffer[255];

            // Get the text out of the profile box
            SendDlgItemMessage(
                hwnd,
                IDC_HOLD_PROFILE_NAME_EDIT,
                WM_GETTEXT,
                255,
                reinterpret_cast<LPARAM>(&buffer)
            );
            std::string profileName = ConvertFromTchar(buffer);

            // Update the profile in the manager and API
            if (
                !this->holdProfileManager.UpdateProfile(this->selectedHoldProfile, profileName, this->selectedHolds)
            ) {
                return;
            }

            // Update the dialog by deleting the string, then re-adding it
            SendDlgItemMessage(
                hwnd,
                IDC_HOLD_PROFILE_LIST,
                LB_DELETESTRING,
                this->selectedHoldProfileIndex,
                NULL
            );
            this->selectedHoldProfileIndex = SendDlgItemMessage(
                hwnd,
                IDC_HOLD_PROFILE_LIST,
                LB_ADDSTRING,
                NULL,
                reinterpret_cast<LPARAM>(buffer)
            );
            SendDlgItemMessage(
                hwnd,
                IDC_HOLD_PROFILE_LIST,
                LB_SETITEMDATA,
                this->selectedHoldProfileIndex,
                this->selectedHoldProfile
            );
            SendDlgItemMessage(
                hwnd,
                IDC_HOLD_PROFILE_LIST,
                LB_SETCURSEL,
                this->selectedHoldProfileIndex,
                NULL
            );

            // Tell the instance that prompted this dialog to open that something has changed.
            this->configurationItem->SelectProfile(this->selectedHoldProfile);
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
                    this->DestroyDialog(hwnd);
                    EndDialog(hwnd, wParam);
                    return TRUE;
                }
                // Buttons pressed
                case WM_COMMAND: {
                    switch (LOWORD(wParam)) {
                        case IDC_HOLD_PROFILE_LIST: {
                            this->SelectProfile(hwnd, wParam);
                            return TRUE;
                        }
                        case HOLD_SELECTOR_OK: {
                            // OK clicked, close the window
                            this->DestroyDialog(hwnd);
                            EndDialog(hwnd, wParam);
                            return TRUE;
                        }
                        case IDC_HOLD_ADD: {
                            this->AddHoldToSelectedList(hwnd);
                            return TRUE;
                        }
                        case IDC_HOLD_REMOVE: {
                            this->RemoveHoldFromSelectedList(hwnd);
                            return TRUE;
                        }
                        case IDC_HOLD_PROFILE_NEW: {
                            this->CreateNewHoldProfile(hwnd);
                            return TRUE;
                        }
                        case IDC_HOLD_PROFILE_UPDATE: {
                            this->UpdateHoldProfile(hwnd);
                            return TRUE;
                        }
                        case IDC_HOLD_PROFILE_DELETE: {
                            this->DeleteHoldProfile(hwnd);
                            return TRUE;
                        }
                        case IDC_HOLD_SELECT_DISPLAY: {
                            this->DisplaySelectedProfile();
                            return TRUE;
                        }
                    }
                }
            }
            return DefWindowProc(hwnd, msg, wParam, lParam);
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
            return dialog ? dialog->_WndProc(hwnd, msg, wParam, lParam) : DefWindowProc(hwnd, msg, wParam, lParam);
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
