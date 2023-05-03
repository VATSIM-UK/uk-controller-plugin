#include "api/ApiConfigurationListener.h"
#include "api/ApiKeyRedirectUrlBuilder.h"
#include "ReplaceApiKeyDialog.h"
#include "dialog/DialogCallArgument.h"
#include "helper/HelperFunctions.h"
#include "windows/WinApiInterface.h"

namespace UKControllerPlugin::Api {

    ReplaceApiKeyDialog::ReplaceApiKeyDialog(
        std::unique_ptr<UKControllerPluginUtils::Api::ApiKeyRedirectUrlBuilder> urlBuilder,
        UKControllerPlugin::Windows::WinApiInterface& windows)
        : urlBuilder(std::move(urlBuilder)), windows(windows)
    {
        assert(this->urlBuilder && "Url builder not set in ReplaceApiKeyDialog");
    }

    LRESULT ReplaceApiKeyDialog::InitDialog(HWND hwnd)
    {
        this->handle = hwnd;
        this->OpenBrowserWindow();
        return TRUE;
    }

    void ReplaceApiKeyDialog::DestroyDialog()
    {
        this->handle = nullptr;
        this->apiConfigListener = nullptr;
    }

    void ReplaceApiKeyDialog::OpenBrowserWindow()
    {
        this->apiConfigListener = std::make_shared<UKControllerPluginUtils::Api::ApiConfigurationListener>();
        windows.OpenWebBrowser(UKControllerPlugin::HelperFunctions::ConvertToWideString(
            urlBuilder->BuildUrl(this->apiConfigListener->Host(), this->apiConfigListener->Port())));
    }

    /*
        Public procedure that windows calls directly
    */
    LRESULT ReplaceApiKeyDialog::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        if (msg == WM_INITDIALOG) {
            LogInfo("Replace api key dialog opened");
            SetWindowLongPtr(
                hwnd,
                GWLP_USERDATA,
                reinterpret_cast<UKControllerPlugin::Dialog::DialogCallArgument*>(lParam)->dialogArgument);
        } else if (msg == WM_DESTROY) {
            SetWindowLongPtr(hwnd, GWLP_USERDATA, NULL);
            LogInfo("Replace api key dialog closed");
        }

        ReplaceApiKeyDialog* dialog = reinterpret_cast<ReplaceApiKeyDialog*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        return dialog ? dialog->_WndProc(hwnd, msg, wParam, lParam) : FALSE;
    }

    /*
        Private procedure that handles calls on the object
    */
    LRESULT ReplaceApiKeyDialog::_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        switch (msg) {
        // Initialise
        case WM_INITDIALOG: {
            this->InitDialog(hwnd);
            return TRUE;
        };
        // Window Closed
        case WM_CLOSE: {
            this->DestroyDialog();
            EndDialog(hwnd, wParam);
            return TRUE;
        }
        // Buttons pressed
        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
            case IDC_API_KEY_REPLACE_CANCEL: {
                this->DestroyDialog();
                EndDialog(hwnd, wParam);
                return TRUE;
            }
            case IDC_API_REOPEN_BROWSER: {
                this->OpenBrowserWindow();
                return TRUE;
            }
            }
        }
        }

        return FALSE;
    }

    void ReplaceApiKeyDialog::OnEvent(const UKControllerPluginUtils::Api::ApiKeyReceivedEvent& event)
    {
        if (!this->handle) {
            return;
        }

        SendMessage(this->handle, WM_CLOSE, NULL, NULL);
    }
} // namespace UKControllerPlugin::Api
