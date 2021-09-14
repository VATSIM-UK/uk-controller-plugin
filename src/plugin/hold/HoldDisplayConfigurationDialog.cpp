#include "pch/pch.h"
#include "hold/HoldDisplayConfigurationDialog.h"
#include "hold/HoldDisplayFunctions.h"
#include "dialog/DialogCallArgument.h"

using UKControllerPlugin::Hold::ConvertToTchar;
using UKControllerPlugin::Dialog::DialogCallArgument;
using UKControllerPlugin::Navaids::NavaidCollection;

namespace UKControllerPlugin {
    namespace Hold {

        /*
            Initialise the dialog
        */
        void HoldDisplayConfigurationDialog::InitDialog(HWND hwnd, LPARAM lParam)
        {
            // One of the ES menu items spawned by the radar screen. If profiles are changed
            // this needs to be reported back to the menu item.
            this->configurationItem = reinterpret_cast<HoldDisplay *>(
                reinterpret_cast<DialogCallArgument *>(lParam)->contextArgument
            );
            
            std::wstring maxText = std::to_wstring(this->configurationItem->GetMaximumLevel());

            // Increment altitude displays by 1000 each time
            UDACCEL accelArray[1];
            accelArray[0] = {
                0,
                1000
            };

            // Max level
            SendDlgItemMessage(
                hwnd,
                IDC_HOLD_MAX_SPIN,
                UDM_SETRANGE32,
                this->minHoldingLevel,
                this->maxHoldingLevel
            );
            SendDlgItemMessage(
                hwnd,
                IDC_HOLD_MAX_SPIN,
                UDM_SETPOS32,
                NULL,
                this->ValueInRange(
                    this->configurationItem->GetMaximumLevel(),
                    this->minHoldingLevel, this->maxHoldingLevel
                )
                ? this->configurationItem->GetMaximumLevel()
                : this->maxHoldingLevel
            );
            SendDlgItemMessage(
                hwnd,
                IDC_HOLD_MAX_SPIN,
                UDM_SETACCEL,
                1,
                reinterpret_cast<LPARAM>(accelArray)
            );

            // Min level
            SendDlgItemMessage(
                hwnd,
                IDC_HOLD_MIN_SPIN,
                UDM_SETRANGE32,
                this->minHoldingLevel,
                this->maxHoldingLevel
            );

            SendDlgItemMessage(
                hwnd,
                IDC_HOLD_MIN_SPIN,
                UDM_SETPOS32,
                NULL,
                this->ValueInRange(
                    this->configurationItem->GetMinimumLevel(),
                    this->minHoldingLevel,
                    this->maxHoldingLevel
                )
                ? this->configurationItem->GetMinimumLevel()
                : this->minHoldingLevel
            );
            SendDlgItemMessage(
                hwnd,
                IDC_HOLD_MIN_SPIN,
                UDM_SETACCEL,
                1,
                reinterpret_cast<LPARAM>(accelArray)
            );
        }

        bool HoldDisplayConfigurationDialog::ValueInRange(int value, int min, int max) const
        {
            return value >= min && value <= max;;
        }

        /*
            Destroy the dialog - clean up
        */
        void HoldDisplayConfigurationDialog::SaveDialog(HWND hwnd)
        {
            int maximumLevel = SendDlgItemMessage(
                hwnd,
                IDC_HOLD_MAX_SPIN,
                UDM_GETPOS32,
                NULL,
                NULL
            );
            maximumLevel = maximumLevel > this->maxHoldingLevel ? this->maxHoldingLevel : maximumLevel;

            int minimumLevel = SendDlgItemMessage(
                hwnd,
                IDC_HOLD_MIN_SPIN,
                UDM_GETPOS32,
                NULL,
                NULL
            );
            minimumLevel = minimumLevel < this->minHoldingLevel ? this->minHoldingLevel : minimumLevel;

            if (maximumLevel < minimumLevel) {
                LogWarning(
                    "Tried to save the hold configuration dialog but the maximum level is less than the minimum"
                );
                return;
            }

            this->configurationItem->SetMaximumLevel((maximumLevel / 1000) * 1000);
            this->configurationItem->SetMinimumLevel((minimumLevel / 1000) * 1000);
        }

        /*
            Private dialog procedure for the hold configuration dialog, should be used
            against a bound instance.
        */
        LRESULT HoldDisplayConfigurationDialog::_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
                        case IDOK: {
                            // OK clicked, close the window after saving
                            this->SaveDialog(hwnd);
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

        /*
            Public dialog procedure for the hold configuration dialog.
        */
        LRESULT HoldDisplayConfigurationDialog::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            if (msg == WM_INITDIALOG) {
                LogInfo("Hold display configuration dialog opened");
                SetWindowLongPtr(
                    hwnd,
                    GWLP_USERDATA,
                    reinterpret_cast<DialogCallArgument *>(lParam)->dialogArgument
                );
            } else if (msg == WM_DESTROY) {
                SetWindowLongPtr(hwnd, GWLP_USERDATA, NULL);
                LogInfo("Hold display configuration dialog closed");
            }

            HoldDisplayConfigurationDialog* dialog = reinterpret_cast<HoldDisplayConfigurationDialog*>(
                GetWindowLongPtr(hwnd, GWLP_USERDATA)
            );
            return dialog ? dialog->_WndProc(hwnd, msg, wParam, lParam) : FALSE;
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
