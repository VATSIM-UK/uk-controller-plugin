#include "FirstTimeApiAuthorisationChecker.h"
#include "api/ApiRequestFactory.h"
#include "api/ApiRequestException.h"
#include "api/ApiSettingsProviderInterface.h"
#include "dialog/DialogManager.h"
#include "windows/WinApiInterface.h"

using UKControllerPluginUtils::Api::ApiRequestException;

namespace UKControllerPlugin::Api {

    void ShowNonReplacementMessage(Windows::WinApiInterface& windows)
    {
        LogInfo("User elected not to set API key after authentication failure");
        windows.OpenMessageBox(
            L"You have elected not to complete API setup at this time. Some functionality of the plugin "
            "may not work as expected.",
            L"UKCP API Config Not Updated",
            MB_OK | MB_ICONWARNING);
    }

    void FirstTimeApiAuthorisationCheck(
        UKControllerPluginUtils::Api::ApiSettingsProviderInterface& settingsProviderInterface,
        Windows::WinApiInterface& windows,
        const Dialog::DialogManager& dialogManager)
    {
        ApiRequest()
            .Get("authorise")
            .Then([]() { LogInfo("Api authorisation check was successful."); })
            .Catch([&windows, &settingsProviderInterface, &dialogManager](const ApiRequestException& exception) {
                LogWarning(
                    "Api authorisation check failed, status code was " +
                    std::to_string(static_cast<uint64_t>(exception.StatusCode())));

                if (UKControllerPluginUtils::Http::IsServerError(exception.StatusCode())) {
                    windows.OpenMessageBox(
                        L"Server error whilst checking API authentication, some functionality may not work as "
                        "expected. If your configuration is otherwise correct, functionality will resume when the "
                        "service is online again.",
                        L"UKCP API Server Error",
                        MB_OK | MB_ICONWARNING);
                    return;
                }

                auto messageResponse = windows.OpenMessageBox(
                    L"API authentication failed. Please click OK to open the UK Controller Plugin website in your web "
                    "browser, log in, and replace your credentials. If this problem persists, please contact the Web "
                    "Services Department.",
                    L"UKCP API Config Invalid",
                    MB_OKCANCEL | MB_ICONWARNING);

                // User didnt want to replace
                if (messageResponse == IDCANCEL) {
                    ShowNonReplacementMessage(windows);
                    return;
                }

                bool apiKeyReplaced = false;
                dialogManager.OpenDialog(IDD_API_KEY_REPLACE, reinterpret_cast<LPARAM>(&apiKeyReplaced));

                // User cancelled the replace procedure
                if (!apiKeyReplaced) {
                    ShowNonReplacementMessage(windows);
                    return;
                }

                // User tried to replace, lets re-check
                FirstTimeApiAuthorisationCheck(settingsProviderInterface, windows, dialogManager);
            })
            .Await();
    }
} // namespace UKControllerPlugin::Api
