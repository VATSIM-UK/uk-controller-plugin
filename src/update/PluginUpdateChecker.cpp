#include "pch/stdafx.h"
#include "update/PluginUpdateChecker.h"
#include "windows/WinApiInterface.h"
#include "api/ApiInterface.h"
#include "api/ApiResponseValidator.h"
#include "api/ApiException.h"
#include "helper/HelperFunctions.h"

using UKControllerPlugin::Windows::WinApiInterface;
using UKControllerPlugin::Api::ApiInterface;
using UKControllerPlugin::Api::ApiResponse;
using UKControllerPlugin::Api::ApiResponseValidator;
using UKControllerPlugin::Api::ApiException;
using UKControllerPlugin::HelperFunctions;

namespace UKControllerPlugin {
    namespace Update {

        /*
            Checks whether an update is available for the plugin and displays messages
            where appropriate.
        */
        int PluginUpdateChecker::CheckForUpdates(
            std::string pluginVersion,
            WinApiInterface & winApi,
            UKControllerPlugin::Api::ApiInterface & api
        ) {

            try {
                return CheckApiResponse(
                    api.UpdateCheck(pluginVersion),
                    pluginVersion,
                    winApi
                );
            } catch (ApiException e) {
                winApi.OpenMessageBox(
                    L"Unable to check plugin version, remote functionality disabled",
                    L"UKCP Error",
                    MB_OK | MB_ICONWARNING
                );
                LogCritical("Unable to check plugin version, the API threw an exception: " + std::string(e.what()));
                return unsupportedVersion;
            }
        }

        /*
            Checks the API response code and logs / opens message boxes
        */
        int PluginUpdateChecker::CheckApiResponse(
            int apiResponse,
            std::string pluginVersion,
            WinApiInterface & winApi
        ) {

            if (apiResponse == ApiInterface::UPDATE_VERSION_DISABLED) {
                std::wstring message = L"This version of the plugin has been withdrawn, ";
                message += L"you need to update to continue using the plugin.\n";
                message += L"Reported version: " + HelperFunctions::ConvertToWideString(pluginVersion);
                winApi.OpenMessageBox(
                    message.c_str(),
                    L"UKCP Update Notification",
                    MB_OK | MB_ICONERROR
                );
                LogCritical("Plugin version is no longer supported. Your version: " + pluginVersion);
                return unsupportedVersion;
            }

            if (apiResponse == ApiInterface::UPDATE_VERSION_NEEDS_UPDATE) {
                std::wstring message = L"An update to the plugin is available.\n";
                message += L"Reported version: " + HelperFunctions::ConvertToWideString(pluginVersion);
                winApi.OpenMessageBox(
                    message.c_str(),
                    L"UKCP Update Notification",
                    MB_OK | MB_ICONINFORMATION
                );
                LogWarning("An update to the plugin is available. Your version: " + pluginVersion);
            } else {
                LogInfo("Plugin is up to date, version: " + pluginVersion);
            }

            return versionAllowed;
        }
    }  // namespace Update
}  // namespace UKControllerPlugin
