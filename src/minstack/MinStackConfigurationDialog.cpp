#include "pch/stdafx.h"
#include "minstack/MinStackConfigurationDialog.h"
#include "dialog/DialogCallArgument.h"
#include "helper/HelperFunctions.h"

using UKControllerPlugin::Dialog::DialogCallArgument;
using UKControllerPlugin::MinStack::MinStackRendererConfiguration;
using UKControllerPlugin::MinStack::MinStackManager;
using UKControllerPlugin::HelperFunctions;

namespace UKControllerPlugin {
    namespace MinStack {

        MinStackConfigurationDialog::MinStackConfigurationDialog(const MinStackManager & manager)
            : manager(manager)
        {

        }

        /*
            Public facing window procedure
        */
        LRESULT MinStackConfigurationDialog::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            if (msg == WM_INITDIALOG) {
                LogInfo("Minstack configuration dialog opened");
                SetWindowLongPtr(
                    hwnd,
                    GWLP_USERDATA,
                    reinterpret_cast<DialogCallArgument *>(lParam)->dialogArgument
                );
            } else if (msg == WM_DESTROY) {
                SetWindowLongPtr(hwnd, GWLP_USERDATA, NULL);
                LogInfo("Minstack configuration dialog closed");
            }

            MinStackConfigurationDialog * dialog = reinterpret_cast<MinStackConfigurationDialog*>(
                GetWindowLongPtr(hwnd, GWLP_USERDATA)
            );
            return dialog ? dialog->_WndProc(hwnd, msg, wParam, lParam) : FALSE;
        }

        /*
            Private window procedure bound to the objects
        */
        LRESULT MinStackConfigurationDialog::_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
                    }
                }
            }

            return FALSE;
        }

        /*
            Initialise the controls from config
        */
        void MinStackConfigurationDialog::InitDialog(HWND hwnd, LPARAM lParam)
        {
            this->config = reinterpret_cast<MinStackRendererConfiguration *>(
                reinterpret_cast<DialogCallArgument *>(lParam)->contextArgument
            );

            // Add the selected MSLs to the existing list
            for (
                MinStackRendererConfiguration::const_iterator it = this->config->cbegin();
                it != this->config->cend();
                ++it
            ) {
                unsigned int itemIndex = SendDlgItemMessage(
                    hwnd,
                    IDC_MINSTACK_LIST,
                    LB_ADDSTRING,
                    NULL,
                    reinterpret_cast<LPARAM>(this->GetListEntryForKey(it->key).c_str())
                );

                SendDlgItemMessage(
                    hwnd,
                    IDC_MINSTACK_LIST,
                    LB_SETITEMDATA,
                    itemIndex,
                    reinterpret_cast<LPARAM>(it->key.c_str())
                );
            }

            // Add all non-active keys to the dropdown
            std::set<std::string> mslKeys = this->manager.GetAllMslKeys();
            for (
                std::set<std::string>::const_iterator it = mslKeys.cbegin();
                it != mslKeys.cend();
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
            Save the controls to config
        */
        void MinStackConfigurationDialog::SaveDialog(HWND hwnd)
        {
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
        std::wstring MinStackConfigurationDialog::GetListEntryForKey(std::string mslKey)
        {
            std::string facility = mslKey.substr(0, 3) == "tma" ? "TMA" : "Airfield";
            std::string identifier = facility == "TMA" ? mslKey.substr(4) : mslKey.substr(9);

            return HelperFunctions::ConvertToWideString(facility + " - " + identifier);
        }
    }  // namespace MinStack
}  // namespace UKControllerPlugin
