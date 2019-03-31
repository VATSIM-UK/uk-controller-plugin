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
            Private dialog procedure for the hold configuration dialog, should be used
            against a bound instance.
        */
        LRESULT HoldConfigurationDialog::_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            switch (msg) {
                // Initialise
                case WM_INITDIALOG: {

                    this->configurationItem = reinterpret_cast<HoldConfigurationMenuItem *>(
                        reinterpret_cast<DialogCallArgument *>(lParam)->contextArgument
                    );

                    // Load the holds
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
                            reinterpret_cast<LPARAM>(ConvertToTchar(it->description))
                        );
                        SendDlgItemMessage(
                            hwnd,
                            IDC_HOLD_SELECTOR,
                            CB_SETITEMDATA,
                            index,
                            (LPARAM) it->identifier
                        );
                    }

                    SendDlgItemMessage(
                        hwnd,
                        IDC_HOLD_SELECTOR,
                        CB_SETCURSEL,
                        0,
                        0
                    );

                    // Load the hold profiles
                    SendDlgItemMessage(
                        hwnd,
                        IDC_HOLD_PROFILE_SELECT,
                        CB_SETMINVISIBLE,
                        10,
                        0
                    );
                    SendDlgItemMessage(
                        hwnd,
                        IDC_HOLD_SELECTOR,
                        CB_LIMITTEXT,
                        255,
                        NULL
                    );

                    // No profiles to add
                    if (this->holdProfileManager.CountProfiles() == 0) {
                        return TRUE;
                    }
                    for (
                        HoldProfileManager::HoldProfiles::const_iterator it = this->holdProfileManager.cbegin();
                        it != this->holdProfileManager.cend();
                        ++it
                    ) {
                        int index = SendDlgItemMessage(
                            hwnd,
                            IDC_HOLD_PROFILE_SELECT,
                            CB_ADDSTRING,
                            NULL,
                            reinterpret_cast<LPARAM>(ConvertToTchar(it->name))
                        );
                        SendDlgItemMessage(
                            hwnd,
                            IDC_HOLD_PROFILE_SELECT,
                            CB_SETITEMDATA,
                            index,
                            (LPARAM)it->id
                        );
                    }

                    // Load the holds into the profile list to get a starting point
                    HoldProfile profile = *this->holdProfileManager.cbegin();
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
                            reinterpret_cast<LPARAM>(ConvertToTchar(hold->description))
                        );
                        SendDlgItemMessage(
                            hwnd,
                            IDC_HOLD_LIST,
                            LB_SETITEMDATA,
                            index,
                            (LPARAM)hold->identifier
                        );
                    }

                    SendDlgItemMessage(
                        hwnd,
                        IDC_HOLD_PROFILE_SELECT,
                        CB_SETCURSEL,
                        0,
                        0
                    );
                    this->selectedHoldProfile = this->holdProfileManager.cbegin()->id;

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

                        case IDC_HOLD_PROFILE_SELECT: {
                            // We're selecting a new profile for holds
                            if (HIWORD(wParam) == CBN_SELCHANGE) {
                                // Change the profile selection index
                                int selectedIndex = SendDlgItemMessage(
                                    hwnd,
                                    IDC_HOLD_PROFILE_SELECT,
                                    CB_GETCURSEL,
                                    NULL,
                                    NULL
                                );
                                int selectedProfileId = SendDlgItemMessage(
                                    hwnd,
                                    IDC_HOLD_PROFILE_SELECT,
                                    CB_GETITEMDATA,
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
                                        reinterpret_cast<LPARAM>(ConvertToTchar(hold->description))
                                    );
                                    SendDlgItemMessage(
                                        hwnd,
                                        IDC_HOLD_LIST,
                                        LB_SETITEMDATA,
                                        index,
                                        (LPARAM) hold->identifier
                                    );
                                }

                                return TRUE;
                            }
                            return DefWindowProc(hwnd, msg, wParam, lParam);
                        }
                        case HOLD_SELECTOR_OK: {
                            // OK clicked, close the window
                            EndDialog(hwnd, wParam);
                            return TRUE;
                        }
                        case IDC_HOLD_ADD: {
                            // Add a hold to the selection list
                            const int selectedIndex = SendDlgItemMessage(
                                hwnd,
                                IDC_HOLD_SELECTOR,
                                CB_GETCURSEL,
                                0,
                                0
                            );

                            if (selectedIndex == LB_ERR) {
                                // They've selected the prompt text, dont carry on
                                return TRUE;
                            }

                            const int selectedHoldId = SendDlgItemMessage(
                                hwnd,
                                IDC_HOLD_SELECTOR,
                                CB_GETITEMDATA,
                                selectedIndex,
                                0
                            );

                            auto hold = this->holds.find(selectedHoldId);
                            if (hold == this->holds.cend()) {
                                return TRUE;
                            }
                            
                            if (!this->selectedHolds.insert(selectedHoldId).second) {
                                return TRUE;
                            }

                            // Add the string to the box and attribute it to a hold
                            int insertedIndex = SendDlgItemMessage(
                                hwnd,
                                IDC_HOLD_LIST,
                                LB_ADDSTRING,
                                NULL,
                                reinterpret_cast<LPARAM>(ConvertToTchar(hold->description))
                            );
                            SendDlgItemMessage(
                                hwnd,
                                IDC_HOLD_LIST,
                                LB_SETITEMDATA,
                                insertedIndex,
                                (LPARAM) selectedHoldId
                            );

                            return TRUE;
                        }
                        case IDC_HOLD_REMOVE: {
                            // Remove a hold from the selection list
                            const int selectedIndex = SendDlgItemMessage(
                                hwnd,
                                IDC_HOLD_SELECTOR,
                                LB_GETCURSEL,
                                0,
                                0
                            );

                            if (selectedIndex == LB_ERR) {
                                // Nothing selected
                                return TRUE;
                            }

                            SendDlgItemMessage(
                                hwnd,
                                IDC_HOLD_LIST,
                                LB_DELETESTRING,
                                selectedIndex,
                                NULL
                            );
                            return TRUE;
                        }
                        case IDC_HOLD_PROFILE_NEW: {
                            // Creating a new hold profile
                            TCHAR buffer[255];

                            // Get the text out of the profile box
                            SendDlgItemMessage(
                                hwnd,
                                IDC_HOLD_PROFILE_SELECT,
                                WM_GETTEXT,
                                255,
                                reinterpret_cast<LPARAM>(&buffer)
                            );
                            std::string profileName = ConvertFromTchar(buffer);

                            // Get the holds.
                            int itemCount = SendDlgItemMessage(
                                hwnd,
                                IDC_HOLD_LIST,
                                LB_GETCOUNT,
                                NULL,
                                NULL
                            );
                            std::set<unsigned int> holds;
                            for (int i = 0; i < itemCount; i++) {
                                int holdId = SendDlgItemMessage(
                                    hwnd,
                                    IDC_HOLD_LIST,
                                    LB_GETITEMDATA,
                                    i,
                                    NULL
                                );
                                holds.insert(holdId);
                            }

                            if (holds.size() == 0) {
                                return TRUE;
                            }

                            // Save the new profile and add to lists
                            int profileId = this->holdProfileManager.SaveNewProfile(profileName, holds);
                            if (profileId == this->holdProfileManager.invalidProfileNewId) {
                                return TRUE;
                            }

                            int newProfileIndex = SendDlgItemMessage(
                                hwnd,
                                IDC_HOLD_PROFILE_SELECT,
                                CB_ADDSTRING,
                                NULL,
                                reinterpret_cast<LPARAM>(buffer)
                            );
                            SendDlgItemMessage(
                                hwnd,
                                IDC_HOLD_PROFILE_SELECT,
                                CB_SETITEMDATA,
                                newProfileIndex,
                                (LPARAM) profileId
                            );

                            // Set the current selection to the new profile
                            SendDlgItemMessage(
                                hwnd,
                                IDC_HOLD_PROFILE_SELECT,
                                CB_SETCURSEL,
                                newProfileIndex,
                                NULL
                            );
                            this->selectedHoldProfile = profileId;
                            this->selectedHoldProfileIndex = newProfileIndex;

                            return TRUE;
                        }
                        case IDC_HOLD_PROFILE_UPDATE: {
                            // They want to update an existing hold profile - don't let them if last selected was not
                            // a real profile
                            if (this->selectedHoldProfileIndex == CB_ERR) {
                                return TRUE;
                            }

                            // Get the holds.
                            int itemCount = SendDlgItemMessage(
                                hwnd,
                                IDC_HOLD_LIST,
                                LB_GETCOUNT,
                                NULL,
                                NULL
                            );
                            std::set<unsigned int> holds;
                            for (int i = 0; i < itemCount; i++) {
                                int holdId = SendDlgItemMessage(
                                    hwnd,
                                    IDC_HOLD_LIST,
                                    LB_GETITEMDATA,
                                    i,
                                    NULL
                                );
                                holds.insert(holdId);
                            }

                            if (holds.size() == 0) {
                                return TRUE;
                            }

                            // Get the new profile name
                            TCHAR buffer[255];

                            // Get the text out of the profile box
                            SendDlgItemMessage(
                                hwnd,
                                IDC_HOLD_PROFILE_SELECT,
                                WM_GETTEXT,
                                255,
                                reinterpret_cast<LPARAM>(&buffer)
                            );
                            std::string profileName = ConvertFromTchar(buffer);

                            // Update it
                            if (
                                !this->holdProfileManager.UpdateProfile(this->selectedHoldProfile, profileName, holds)
                            ) {
                                return TRUE;
                            }

                            // Update the dialog by deleting the string, then re-adding it
                            SendDlgItemMessage(
                                hwnd,
                                IDC_HOLD_PROFILE_SELECT,
                                CB_DELETESTRING,
                                this->selectedHoldProfileIndex,
                                NULL
                            );
                            this->selectedHoldProfileIndex = SendDlgItemMessage(
                                hwnd,
                                IDC_HOLD_PROFILE_SELECT,
                                CB_ADDSTRING,
                                NULL,
                                reinterpret_cast<LPARAM>(buffer)
                            );
                            SendDlgItemMessage(
                                hwnd,
                                IDC_HOLD_PROFILE_SELECT,
                                CB_SETITEMDATA,
                                this->selectedHoldProfileIndex,
                                this->selectedHoldProfile
                            );
                            SendDlgItemMessage(
                                hwnd,
                                IDC_HOLD_PROFILE_SELECT,
                                CB_SETCURSEL,
                                this->selectedHoldProfileIndex,
                                NULL
                            );

                            this->configurationItem->SelectProfile(this->selectedHoldProfile);
                            return TRUE;
                        }
                        case IDC_HOLD_PROFILE_DELETE: {
                            // They want to delete a hold profile
                            const int selectedIndex = SendDlgItemMessage(
                                hwnd,
                                IDC_HOLD_PROFILE_SELECT,
                                CB_GETCURSEL,
                                NULL,
                                NULL
                            );

                            // Cant delete an invalid entry
                            if (selectedIndex == CB_ERR) {
                                return TRUE;
                            }

                            const int profileId = SendDlgItemMessage(
                                hwnd,
                                IDC_HOLD_PROFILE_SELECT,
                                CB_GETITEMDATA,
                                selectedIndex,
                                NULL
                            );

                            if (!this->holdProfileManager.DeleteProfile(profileId)) {
                                return TRUE;
                            }

                            // Remove from the combobox
                            SendDlgItemMessage(
                                hwnd,
                                IDC_HOLD_PROFILE_SELECT,
                                CB_DELETESTRING,
                                selectedIndex,
                                NULL
                            );

                            // Reposition the selection
                            SendDlgItemMessage(
                                hwnd,
                                IDC_HOLD_PROFILE_SELECT,
                                CB_SETCURSEL,
                                0,
                                0
                            );
                            this->selectedHoldProfile = SendDlgItemMessage(
                                hwnd,
                                IDC_HOLD_PROFILE_SELECT,
                                CB_GETITEMDATA,
                                0,
                                NULL
                            );
                            this->selectedHoldProfileIndex = 0;
                            return TRUE;
                        }
                        case IDC_HOLD_SELECT_DISPLAY: {
                            // They want to display the hold configuration

                            for (
                                std::set<unsigned int>::const_iterator it = this->selectedHolds.cbegin();
                                it != this->selectedHolds.cend();
                                ++it
                            ) {
                                auto hold = this->holds.find(*it);
                                if (hold == this->holds.cend()) {
                                    LogWarning("Tried to load invalid hold " + *it);
                                    continue;
                                }

                                this->configurationItem->SelectProfile(this->selectedHoldProfile);
                            }
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
            }
            else if (msg == WM_DESTROY) {
                SetWindowLongPtr(hwnd, GWLP_USERDATA, NULL);
                LogInfo("Hold configuration dialog closed");
            }

            HoldConfigurationDialog * dialog = reinterpret_cast<HoldConfigurationDialog*>(
                GetWindowLongPtr(hwnd, GWLP_USERDATA)
            );
            return dialog ? dialog->_WndProc(hwnd, msg, wParam, lParam) : DefWindowProc(hwnd, msg, wParam, lParam);
        }
    }  // namespace Hold
} // namespace UKControllerPlugin
