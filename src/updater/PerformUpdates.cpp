#include "updater/pch.h"
#include "updater/PerformUpdates.h"
#include "helper/HelperFunctions.h"
#include "api/ApiInterface.h"
#include "windows/WinApiInterface.h"
#include "curl/CurlInterface.h"
#include "update/UpdateBinaries.h"
#include "data/PluginDataLocations.h"

using UKControllerPlugin::HelperFunctions;
using UKControllerPlugin::Api::ApiInterface;
using UKControllerPlugin::Windows::WinApiInterface;
using UKControllerPlugin::Curl::CurlInterface;

void CheckForUpdates(
    const ApiInterface& api,
    WinApiInterface& windows,
    CurlInterface& curl
)
{
    try {
        const nlohmann::json versionDetails = UKControllerPlugin::GetUpdateData(api);
        if (UpdateRequired(windows, versionDetails)) {
            PerformUpdates(curl, windows, versionDetails);
            DisplayUpdateNotification(
                windows, HelperFunctions::ConvertToWideString(GetVersionFromJson(versionDetails))
            );
        }
    } catch (std::exception exception) {
        std::wstring message = std::wstring(HelperFunctions::ConvertToWideString(exception.what())) + L"\r\n";
        message += L"Plugin will attempt to load with previously downloaded version.";

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
        throw std::exception("Failed to update UKCP binaries.");
    }

    UpdateLockfile(windows, GetVersionFromJson(versionDetails));
}

bool UpdateRequired(WinApiInterface& windows, const nlohmann::json& versionDetails)
{
    return !windows.FileExists(GetVersionLockfileLocation()) ||
        windows.ReadFromFile(GetVersionLockfileLocation()) != GetVersionFromJson(versionDetails);
}

void MoveOldUpdaterBinary(WinApiInterface& windows)
{
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
    std::wstring message = L"The UK Controller Plugin has been automatically updated to version " + version + L".\r\n";
    message += L"Please consult the changelog (available through the OP menu) for more details";

    windows.OpenMessageBox(
        message.c_str(),
        L"UKCP Automatic Update Complete",
        MB_OK | MB_ICONINFORMATION
    );
}

std::wstring GetOldUpdaterLocation()
{
    return GetBinariesFolderRelativePath() + L"/UKControllerPluginUpdater.dll.old";
}

void UpdateLockfile(WinApiInterface& windows, std::string version)
{
    windows.WriteToFile(GetVersionLockfileLocation(), version, true);
}
