#include "pch/stdafx.h"
#include "update/PluginUpdateChecker.h"
#include "windows/WinApiInterface.h"
#include "api/ApiInterface.h"
#include "api/ApiResponseValidator.h"
#include "api/ApiException.h"

using UKControllerPlugin::Windows::WinApiInterface;
using UKControllerPlugin::Api::ApiInterface;
using UKControllerPlugin::Api::ApiResponse;
using UKControllerPlugin::Api::ApiResponseValidator;
using UKControllerPlugin::Api::ApiException;

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
                return PluginUpdateChecker::CheckApiResponse(
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
                return PluginUpdateChecker::unsupportedVersion;
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
                winApi.OpenMessageBox(
                    L"This version of the plugin has been withdrawn, you need to update to continue using the plugin.",
                    L"UKCP Update Notification",
                    MB_OK | MB_ICONERROR
                );
                LogCritical("Plugin version is no longer supported. Your version: " + pluginVersion);
                return PluginUpdateChecker::unsupportedVersion;
            }

            if (apiResponse == ApiInterface::UPDATE_VERSION_NEEDS_UPDATE) {
                winApi.OpenMessageBox(
                    L"An update to the plugin is available.",
                    L"UKCP Update Notification",
                    MB_OK | MB_ICONINFORMATION
                );
                LogWarning("An update to the plugin is available. Your version: " + pluginVersion);
            } else {
                LogInfo("Plugin is up to date, version: " + pluginVersion);
            }

            return PluginUpdateChecker::versionAllowed;
        }
    }  // namespace Update
}  // namespace UKControllerPlugin
