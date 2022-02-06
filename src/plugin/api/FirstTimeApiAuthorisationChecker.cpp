#include "FirstTimeApiAuthorisationChecker.h"
#include "api/ApiRequestFactory.h"
#include "api/ApiRequestException.h"
#include "api/ApiSettingsProviderInterface.h"
#include "windows/WinApiInterface.h"

using UKControllerPluginUtils::Api::ApiRequestException;

namespace UKControllerPlugin::Api {

    void FirstTimeApiAuthorisationCheck(
        UKControllerPluginUtils::Api::ApiSettingsProviderInterface& settingsProviderInterface,
        Windows::WinApiInterface& windows)
    {
        ApiRequest()
            .Get("authorise")
            .Then([]() { LogInfo("Api authorisation check was successful."); })
            .Catch([&windows, &settingsProviderInterface](const ApiRequestException& exception) {
                LogWarning(
                    "Api authorisation check failed, status code was " +
                    std::to_string(static_cast<uint64_t>(exception.StatusCode())));

                if (UKControllerPluginUtils::Http::IsServerError(exception.StatusCode())) {
                    windows.OpenMessageBox(
                        L"Server error whilst checking API authentication, some functionality may not work as "
                        "expected. If your configuration is otherwise correct, functionality will resume when the "
                        "service is online again.",
                        L"UKCP API Config Invalid",
                        MB_OK | MB_ICONWARNING);
                    return;
                }

                auto messageResponse = windows.OpenMessageBox(
                    L"API authentication failed. Please re-download your credentails from the VATSIM UK website "
                    "and click OK to try again. If this problem persists, please contact the Web Services Department.",
                    L"UKCP API Config Invalid",
                    MB_OKCANCEL | MB_ICONWARNING);

                if (messageResponse == IDCANCEL || !settingsProviderInterface.Reload()) {
                    LogInfo("User elected not to set API key after authentication failure");
                    windows.OpenMessageBox(
                        L"You have elected not to complete API setup at this time. Some functionality of the plugin "
                        "may not work as expected.",
                        L"UKCP API Config Invalid",
                        MB_OK | MB_ICONWARNING);
                    return;
                }

                FirstTimeApiAuthorisationCheck(settingsProviderInterface, windows);
            })
            .Await();
    }
} // namespace UKControllerPlugin::Api
