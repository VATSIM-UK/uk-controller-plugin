#include "ApiConfigurationMenuItem.h"
#include "api/ApiRequestException.h"
#include "api/ApiRequestFactory.h"
#include "api/Response.h"
#include "dialog/DialogManager.h"

using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::Windows::WinApiInterface;
using UKControllerPluginUtils::Api::ApiRequestException;
using UKControllerPluginUtils::Api::ApiSettingsProviderInterface;
using UKControllerPluginUtils::Api::Response;

namespace UKControllerPlugin::Api {
    ApiConfigurationMenuItem::ApiConfigurationMenuItem(
        Dialog::DialogManager& dialogManager, Windows::WinApiInterface& windows, int menuCallbackId)
        : dialogManager(dialogManager), windows(windows), menuCallbackId(menuCallbackId)
    {
    }

    void ApiConfigurationMenuItem::Configure(int functionId, std::string subject, RECT area)
    {
        bool apiKeyReplaced = false;
        dialogManager.OpenDialog(IDD_API_KEY_REPLACE, reinterpret_cast<LPARAM>(&apiKeyReplaced));
        if (!apiKeyReplaced) {
            return;
        };

        ApiRequest()
            .Get("authorise")
            .Then([this]() {
                LogInfo("Api configuration updated successfully");
                windows.OpenMessageBox(
                    L"API configuration has been replaced sucessfully",
                    L"Configuration Updated",
                    MB_OK | MB_ICONINFORMATION);
            })
            .Catch([this](const ApiRequestException& exception) {
                if (UKControllerPluginUtils::Http::IsAuthenticationError(exception.StatusCode())) {
                    windows.OpenMessageBox(
                        L"API authentication failed. Please try to replace your credentials again. "
                        "If this problem persists, please contact the Web Services Department. Some "
                        "functionality such as stand and squawk allocations may not work as expected.",
                        L"UKCP API Config Invalid",
                        MB_OK | MB_ICONWARNING);
                } else if (UKControllerPluginUtils::Http::IsServerError(exception.StatusCode()))
                    windows.OpenMessageBox(
                        L"Unable to perform API config check as the API responded with an error. Please try again "
                        L"later. Some functionality such as stand and squawk allocations may not work as expected.",
                        L"Server Error",
                        MB_OK | MB_ICONERROR);
            });
    }

    /*
        Return the menu item to be displayed
    */
    auto ApiConfigurationMenuItem::GetConfigurationMenuItem() const -> UKControllerPlugin::Plugin::PopupMenuItem
    {
        PopupMenuItem returnVal;
        returnVal.firstValue = this->itemDescription;
        returnVal.secondValue = "";
        returnVal.callbackFunctionId = this->menuCallbackId;
        returnVal.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
        returnVal.disabled = false;
        returnVal.fixedPosition = false;
        return returnVal;
    }
} // namespace UKControllerPlugin::Api
