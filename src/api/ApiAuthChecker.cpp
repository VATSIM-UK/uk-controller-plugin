#include "pch/stdafx.h"
#include "api/ApiAuthChecker.h"
#include "api/ApiInterface.h"
#include "api/ApiResponse.h"
#include "windows/WinApiInterface.h"
#include "api/ApiNotAuthorisedException.h"

using UKControllerPlugin::Api::ApiInterface;
using UKControllerPlugin::Api::ApiResponse;
using UKControllerPlugin::Windows::WinApiInterface;
using UKControllerPlugin::Api::ApiNotAuthorisedException;

namespace UKControllerPlugin {
    namespace Api {

        /*
            Returns true if the the plugin is authenticated with the API, that is, the API is a teapot.
        */
        bool ApiAuthChecker::IsAuthorised(const ApiInterface & api, WinApiInterface & windows)
        {
            try {
                if (api.CheckApiAuthorisation()) {
                    LogInfo("Successfully authenticated with the Web API");
                }
                return true;
            } catch (ApiNotAuthorisedException notAuth) {
                std::wstring message;
                message += L"Unable to authenticate with the API. Core functionality will be disabled.\r\n\r\n";
                message += L"Please go to https://vatsim.uk/ukcp to download your personal access token and use the ";
                message += L"\"Replace Personal API Configuration\" option in the plugin menu to install it.";
                windows.OpenMessageBox(
                    message.c_str(),
                    L"UKCP API Authentication Warning",
                    MB_OK | MB_ICONWARNING
                );
                LogCritical("API authorisation failed: " + std::string(notAuth.what()));
                return false;
            } catch (ApiException api) {
                // Something weird is going on.
                std::wstring message;
                message += L"Unable to access the API. Core functionality will be disabled. \r\n\r\n";
                message += L"If the problem persists, please contact the VATSIM UK Web Services Department.";
                windows.OpenMessageBox(
                    message.c_str(),
                    L"UKCP API Availability Warning",
                    MB_OK | MB_ICONWARNING
                );
                LogCritical(
                    "The API returned a non-standard response when authenticating: " + std::string(api.what())
                );
                return false;
            }
        }
    }  // namespace Api
}  // namespace UKControllerPlugin
