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
                windows.OpenMessageBox(
                    L"API authorisation failed. The vast majority of functionality will be disabled.",
                    L"UKCP Warning",
                    MB_OK | MB_ICONWARNING
                );
                LogCritical("API authorisation failed: " + std::string(notAuth.what()));
                return false;
            } catch (ApiException api) {
                // Something weird is going on.
                windows.OpenMessageBox(
                    L"Issues detected with the API, remote functionality is disabled.",
                    L"UKCP Warning",
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
