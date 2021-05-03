#include "updater/pch.h"
#include "updater/PerformUpdates.h"
#include "helper/HelperFunctions.h"
#include "api/ApiInterface.h"
#include "windows/WinApiInterface.h"
#include "curl/CurlInterface.h"
#include "update/UpdateBinaries.h"
#include "data/PluginDataLocations.h"
#include "update/LoadChangelog.h"

using UKControllerPlugin::HelperFunctions;
using UKControllerPlugin::Api::ApiInterface;
using UKControllerPlugin::Windows::WinApiInterface;
using UKControllerPlugin::Curl::CurlInterface;

void CheckForUpdates(
    const ApiInterface& api,
    WinApiInterface& windows,
    CurlInterface& curl,
    bool duplicatePlugin
)
{
    if (duplicatePlugin) {
        LogInfo("Skipping updates as plugin is duplicate");
        return;
    }

    try {
        const nlohmann::json versionDetails = UKControllerPlugin::GetUpdateData(api);
        std::string version = GetVersionFromJson(versionDetails);
        if (UpdateRequired(windows, versionDetails)) {
            LogInfo("Plugin update available, newest version " + version);
            PerformUpdates(curl, windows, versionDetails);
            DisplayUpdateNotification(
                windows,
                HelperFunctions::ConvertToWideString(version)
            );
        } else {
            LogInfo("Plugin is up to date at version " + version);
        }
    } catch (std::exception exception) {
        std::wstring message = std::wstring(HelperFunctions::ConvertToWideString(exception.what())) + L"\r\n";
        message += L"Plugin will attempt to load with previously downloaded version.";
        LogError("Exception when performing updates, message: " + std::string(exception.what()));

        windows.OpenMessageBox(
            message.c_str(),
            L"UKCP Automatic Update Failed",
            MB_OK | MB_ICONSTOP
        );
    }
}

void PerformUpdates(
    CurlInterface& curl,
    WinApiInterface& windows,
    const nlohmann::json& versionDetails
)
{
    MoveOldUpdaterBinary(windows);
    bool updatedSuccessfully = UKControllerPlugin::DownloadCoreLibrary(versionDetails, windows, curl) &&
        UKControllerPlugin::DownloadUpdater(versionDetails, windows, curl);

    if (!updatedSuccessfully) {
        LogError("Error when updating plugin binaries");
        throw std::exception("Failed to update UKCP binaries.");
    }

    UpdateLockfile(windows, GetVersionFromJson(versionDetails));
}

bool UpdateRequired(WinApiInterface& windows, const nlohmann::json& versionDetails)
{
    return !windows.FileExists(GetVersionLockfileLocation()) ||
        windows.ReadFromFile(GetVersionLockfileLocation()) != GetVersionFromJson(versionDetails) ||
        !windows.FileExists(GetCoreBinaryRelativePath());
}

void MoveOldUpdaterBinary(WinApiInterface& windows)
{
    LogInfo("Moving updater binary to old version");
    windows.MoveFileToNewLocation(GetUpdaterBinaryRelativePath(), GetOldUpdaterBinaryRelativePath());
}

std::wstring GetVersionLockfileLocation()
{
    return L"version.lock";
}

std::string GetVersionFromJson(const nlohmann::json& versionDetails)
{
    return versionDetails.at("version").get<std::string>();
}

void DisplayUpdateNotification(WinApiInterface& windows, std::wstring version)
{
    std::wstring message = L"The UK Controller Plugin has been automatically updated to version " + version +
        L".\r\n\r\n";
    message +=
        L"You can view the changelog at any time by opening the OP menu and selecting the relevant item.\r\n\r\n";
    message += L"Would you like to view the changelog now?";

    int gotoChangelog = windows.OpenMessageBox(
        message.c_str(),
        L"UKCP Automatic Update Complete",
        MB_YESNO | MB_ICONINFORMATION
    );

    if (gotoChangelog == IDYES) {
        UKControllerPlugin::Update::LoadChangelog(windows);
    }
}

std::wstring GetOldUpdaterLocation()
{
    return GetBinariesFolderRelativePath() + L"/UKControllerPluginUpdater.dll.old";
}

void UpdateLockfile(WinApiInterface& windows, std::string version)
{
    LogInfo("Updating version lockfile");
    windows.WriteToFile(GetVersionLockfileLocation(), version, true, false);
}
