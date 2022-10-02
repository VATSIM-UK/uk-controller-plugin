#include "api/ApiAuthChecker.h"
#include "api/ApiInterface.h"
#include "api/ApiResponse.h"
#include "windows/WinApiInterface.h"
#include "api/ApiNotAuthorisedException.h"
#include "api/LocateApiSettings.h"
#include "setting/SettingRepository.h"

using UKControllerPlugin::Api::ApiInterface;
using UKControllerPlugin::Api::ApiResponse;
using UKControllerPlugin::Api::ApiRequestBuilder;
using UKControllerPlugin::Windows::WinApiInterface;
using UKControllerPlugin::Api::ApiNotAuthorisedException;
using UKControllerPlugin::Api::UserRequestedKeyUpdateNoPrompts;
using UKControllerPlugin::Setting::SettingRepository;

namespace UKControllerPlugin {
    namespace Api {

        /*
            Returns true if the the plugin is authenticated with the API, that is, the API is a teapot.
        */
        bool ApiAuthChecker::IsAuthorised(
            ApiInterface & api,
            WinApiInterface & windows,
            SettingRepository & settings
        ) {
            try {
                if (api.CheckApiAuthorisation()) {
                    LogInfo("Successfully authenticated with the Web API");
                }
                return true;
            } catch (ApiNotAuthorisedException notAuth) {
                std::wstring message;
                message += L"Unable to authenticate with the API. Core functionality will be disabled.\r\n\r\n";
                message += L"Please go to https://vatsim.uk/ukcp to download your personal access token. \r\n";
                message += L"Once you have done this, please click OK and use the next dialog to install the key.";
                int messageResponse = windows.OpenMessageBox(
                    message.c_str(),
                    L"UKCP API Authentication Warning",
                    MB_OKCANCEL | MB_ICONWARNING
                );
                LogCritical("API authorisation failed: " + std::string(notAuth.what()));

                if (messageResponse == IDCANCEL) {
                    LogInfo("User elected not to replace API key");
                    return false;
                }

                // Give them the chance to load their keys again
                UserRequestedKeyUpdateNoPrompts(windows, settings);
                api.SetApiDomain(settings.GetSetting("api-url"));
                api.SetApiKey(settings.GetSetting("api-key"));

                return ApiAuthChecker::IsAuthorised(api, windows, settings);
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
