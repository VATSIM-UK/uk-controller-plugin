#include "ApiConfigurationMenuItem.h"
#include "api/ApiRequestException.h"
#include "api/ApiRequestFactory.h"
#include "api/Response.h"
#include "api/ApiSettingsProviderInterface.h"

using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::Windows::WinApiInterface;
using UKControllerPluginUtils::Api::ApiRequestException;
using UKControllerPluginUtils::Api::ApiSettingsProviderInterface;
using UKControllerPluginUtils::Api::Response;

namespace UKControllerPlugin::Api {
    ApiConfigurationMenuItem::ApiConfigurationMenuItem(
        UKControllerPluginUtils::Api::ApiSettingsProviderInterface& provider,
        Windows::WinApiInterface& windows,
        int menuCallbackId)
        : provider(provider), windows(windows), menuCallbackId(menuCallbackId)
    {
    }

    void ApiConfigurationMenuItem::Configure(int functionId, std::string subject, RECT area)
    {
        if (!provider.Reload()) {
            return;
        };

        ApiRequest()
            .Get("authorise")
            .Then([]() { LogInfo("Api configuration updated successfully"); })
            .Catch([this](const ApiRequestException& exception) {
                if (UKControllerPluginUtils::Http::IsAuthenticationError(exception.StatusCode())) {
                    windows.OpenMessageBox(
                        L"API authentication failed. Please re-download your credentails from the VATSIM UK website "
                        "and try again. If this problem persists, please contact the Web Services Department. Some "
                        "functionality such as stand and squawk allocations may not work as expected.",
                        L"UKCP API Config Invalid",
                        MB_OK | MB_ICONWARNING);
                } else if (UKControllerPluginUtils::Http::IsServerError(exception.StatusCode()))
                    windows.OpenMessageBox(
                        L"Unable to perform API config check as the API responded with an error. Please try again "
                        L"later. Some functionality such as stand and squawk allocations may not work as expected.",
                        L"UKCP API Config Invalid",
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
