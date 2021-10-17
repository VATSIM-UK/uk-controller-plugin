#include "MissedApproachConfigurationDialog.h"
#include "MissedApproachOptions.h"
#include "MissedApproachRenderOptions.h"
#include "MissedApproachUserSettingHandler.h"
#include "airfield/AirfieldCollection.h"
#include "dialog/DialogCallArgument.h"

using UKControllerPlugin::Dialog::DialogCallArgument;

namespace UKControllerPlugin::MissedApproach {

    MissedApproachConfigurationDialog::MissedApproachConfigurationDialog(
        std::shared_ptr<MissedApproachUserSettingHandler> missedApproachUserSettings,
        const Airfield::AirfieldCollection& airfields)
        : missedApproachUserSettings(std::move(missedApproachUserSettings)), airfields(airfields)
    {
        auto test = this->airfields.GetSize();
        test = test / 2;
    }
    
    auto MissedApproachConfigurationDialog::InitDialog(HWND hwnd, LPARAM lParam) -> LRESULT
    {
        this->renderOptions = *reinterpret_cast<std::shared_ptr<MissedApproachRenderOptions>*>(
            reinterpret_cast<Dialog::DialogCallArgument*>(lParam)->contextArgument
        );
        
        // Set global settings
        // Play sound
        CheckDlgButton(
            hwnd,
            MISSED_APPROACH_PLAY_SOUND,
            this->missedApproachUserSettings->GetOptions()->AudioAlert() ? BST_CHECKED : BST_UNCHECKED
        );
        
        // Set per ASR-settings
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
        
        MissedApproachConfigurationDialog* dialog = reinterpret_cast<MissedApproachConfigurationDialog*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        return dialog != nullptr ? dialog->PrivateWindowProcedure(hwnd, msg, wParam, lParam) : FALSE;
    }
    
    auto MissedApproachConfigurationDialog::PrivateWindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
                                                 -> LRESULT
    {
        switch (msg) {
        // Initialise
        case WM_INITDIALOG: {
            //this->InitDialog(hwnd, lParam);
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
