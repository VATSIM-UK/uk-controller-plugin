#include "MissedApproachConfigurationDialog.h"
#include "MissedApproachOptions.h"
#include "MissedApproachRenderOptions.h"
#include "MissedApproachUserSettingHandler.h"
#include "airfield/AirfieldModel.h"
#include "airfield/AirfieldCollection.h"
#include "dialog/DialogCallArgument.h"
#include "helper/HelperFunctions.h"

using UKControllerPlugin::HelperFunctions;
using UKControllerPlugin::Dialog::DialogCallArgument;

namespace UKControllerPlugin::MissedApproach {

    MissedApproachConfigurationDialog::MissedApproachConfigurationDialog(
        std::shared_ptr<MissedApproachUserSettingHandler> missedApproachUserSettings,
        const Airfield::AirfieldCollection& airfields)
        : missedApproachUserSettings(std::move(missedApproachUserSettings)), airfields(airfields)
    {
    }

    auto MissedApproachConfigurationDialog::InitDialog(HWND hwnd, LPARAM lParam) -> LRESULT
    {
        this->renderOptions = *reinterpret_cast<std::shared_ptr<MissedApproachRenderOptions>*>(
            reinterpret_cast<Dialog::DialogCallArgument*>(lParam)->contextArgument);

        // Set global settings
        // Play sound
        CheckDlgButton(
            hwnd,
            MISSED_APPROACH_PLAY_SOUND,
            this->missedApproachUserSettings->GetOptions()->AudioAlert() ? BST_CHECKED : BST_UNCHECKED);

        // Play sound for same user
        CheckDlgButton(
            hwnd,
            MISSED_APPROACH_SOUND_USER,
            this->missedApproachUserSettings->GetOptions()->AudioAlertForCurrentUser() ? BST_CHECKED : BST_UNCHECKED);

        // Service provision list
        HWND serviceList = GetDlgItem(hwnd, MISSED_APPROACH_SERVICE_LIST);
        ListView_SetExtendedListViewStyle(serviceList, LVS_EX_CHECKBOXES);

        const auto& selectedServices = this->missedApproachUserSettings->GetOptions()->ServiceProvisions();

        LVITEM item;
        item.mask = LVIF_TEXT;
        item.iItem = 0;
        item.iSubItem = 0;
        for (const auto& serviceTypeMapping : this->serviceTypeMap) {
            std::wstring text = HelperFunctions::ConvertToWideString(serviceTypeMapping.second);
            item.pszText = (LPWSTR)text.c_str();
            ListView_InsertItem(serviceList, &item);

            ListView_SetCheckState(
                serviceList, item.iItem, (serviceTypeMapping.first & selectedServices) == serviceTypeMapping.first);

            item.iItem++;
        }

        // Airfield list
        HWND airfieldList = GetDlgItem(hwnd, MISSED_APPROACH_AIRFIELD_LIST);
        ListView_SetExtendedListViewStyle(airfieldList, LVS_EX_CHECKBOXES);

        const auto& selectedAirfields = this->missedApproachUserSettings->GetOptions()->Airfields();

        LVITEM airfieldItem;
        airfieldItem.mask = LVIF_TEXT;
        airfieldItem.iItem = 0;
        airfieldItem.iSubItem = 0;
        this->airfields.ForEach(
            [&airfieldItem, &airfieldList, &selectedAirfields](const Airfield::AirfieldModel& airfield) {
                std::wstring text = HelperFunctions::ConvertToWideString(airfield.Icao());
                airfieldItem.pszText = (LPWSTR)text.c_str();
                ListView_InsertItem(airfieldList, &airfieldItem);

                ListView_SetCheckState(
                    airfieldList,
                    airfieldItem.iItem,
                    std::find(selectedAirfields.cbegin(), selectedAirfields.cend(), airfield.Icao()) !=
                        selectedAirfields.end());

                airfieldItem.iItem++;
            });

        // Set per ASR-settings
        const auto renderMode = this->renderOptions->Mode();

        // Line render mode
        CheckDlgButton(
            hwnd,
            MISSED_APPROACH_LINE,
            (renderMode & MissedApproachRenderMode::Line) == MissedApproachRenderMode::Line ? BST_CHECKED
                                                                                            : BST_UNCHECKED);

        // Circle
        CheckDlgButton(
            hwnd,
            MISSED_APPROACH_CIRCLE,
            (renderMode & MissedApproachRenderMode::Circle) == MissedApproachRenderMode::Circle ? BST_CHECKED
                                                                                                : BST_UNCHECKED);

        // Draw duration
        const auto drawDuration = std::to_wstring(this->renderOptions->Duration().count());
        SendDlgItemMessage(hwnd, MISSED_APPROACH_DRAW_DURATION, EM_SETLIMITTEXT, 1, NULL);
        SendDlgItemMessage(
            hwnd, MISSED_APPROACH_DRAW_DURATION, WM_SETTEXT, NULL, reinterpret_cast<LPARAM>(drawDuration.c_str()));

        return TRUE;
    }

    auto MissedApproachConfigurationDialog::SaveDialog(HWND hwnd) -> LRESULT
    {
        // Play sound
        this->missedApproachUserSettings->SetAudioAlert(
            IsDlgButtonChecked(hwnd, MISSED_APPROACH_PLAY_SOUND) == BST_CHECKED);

        // Play sound for user
        this->missedApproachUserSettings->SetAudioAlertForCurrentUser(
            IsDlgButtonChecked(hwnd, MISSED_APPROACH_SOUND_USER) == BST_CHECKED);

        // Airfields
        HWND airfieldList = GetDlgItem(hwnd, MISSED_APPROACH_AIRFIELD_LIST);

        std::vector<std::string> selectedAirfields;
        int numAirfields = ListView_GetItemCount(airfieldList);
        for (int item = 0; item < numAirfields; ++item) {
            if (ListView_GetCheckState(airfieldList, item)) {
                TCHAR buffer[5];
                ListView_GetItemText(airfieldList, item, 0, buffer, 5);
                selectedAirfields.push_back(HelperFunctions::ConvertToRegularString(buffer));
            }
        }
        this->missedApproachUserSettings->SetAirfields(selectedAirfields);

        // Services
        HWND serviceList = GetDlgItem(hwnd, MISSED_APPROACH_SERVICE_LIST);

        Ownership::ServiceType provisions = static_cast<Ownership::ServiceType>(0u);
        int numServices = ListView_GetItemCount(serviceList);
        for (int item = 0; item < numServices; ++item) {
            if (ListView_GetCheckState(serviceList, item)) {
                TCHAR buffer[100];
                ListView_GetItemText(serviceList, item, 0, buffer, 100);
                const auto serviceString = HelperFunctions::ConvertToRegularString(buffer);
                auto service = std::find_if(
                    this->serviceTypeMap.cbegin(),
                    this->serviceTypeMap.cend(),
                    [&serviceString](const std::pair<Ownership::ServiceType, std::string>& pair) -> bool {
                        return pair.second == serviceString;
                    });
                provisions = provisions | service->first;
            }
        }
        this->missedApproachUserSettings->SetServiceProvisions(provisions);

        // Per-ASR settings
        // Mode
        MissedApproachRenderMode mode = MissedApproachRenderMode::None;
        if (IsDlgButtonChecked(hwnd, MISSED_APPROACH_LINE) == BST_CHECKED) {
            mode = mode | MissedApproachRenderMode::Line;
        }

        if (IsDlgButtonChecked(hwnd, MISSED_APPROACH_CIRCLE) == BST_CHECKED) {
            mode = mode | MissedApproachRenderMode::Circle;
        }

        this->renderOptions->SetMode(mode);

        // Duration
        TCHAR buffer[2];
        SendDlgItemMessage(hwnd, MISSED_APPROACH_DRAW_DURATION, WM_GETTEXT, 2, reinterpret_cast<LPARAM>(buffer));
        this->renderOptions->SetDuration(std::chrono::seconds(std::stoi(buffer)));

        return TRUE;
    }

    auto CALLBACK MissedApproachConfigurationDialog::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        -> LRESULT
    {
        if (msg == WM_INITDIALOG) {
            LogInfo("Missed approach configuration dialog opened");
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<DialogCallArgument*>(lParam)->dialogArgument);
        } else if (msg == WM_DESTROY) {
            SetWindowLongPtr(hwnd, GWLP_USERDATA, NULL);
            LogInfo("Missed approach configuration dialog closed");
        }

        MissedApproachConfigurationDialog* dialog =
            reinterpret_cast<MissedApproachConfigurationDialog*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        return dialog != nullptr ? dialog->PrivateWindowProcedure(hwnd, msg, wParam, lParam) : FALSE;
    }

    auto MissedApproachConfigurationDialog::PrivateWindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        -> LRESULT
    {
        switch (msg) {
        // Initialise
        case WM_INITDIALOG: {
            this->InitDialog(hwnd, lParam);
            return TRUE;
        };
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
