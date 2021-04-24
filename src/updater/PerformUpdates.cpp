#include "updater/pch.h"
#include "updater/PerformUpdates.h"
#include "helper/HelperFunctions.h"
#include "api/ApiInterface.h"
#include "windows/WinApiInterface.h"
#include "curl/CurlInterface.h"
#include "update/UpdateBinaries.h"
#include "data/PluginDataLocations.h"

using UKControllerPlugin::HelperFunctions;

void CheckForUpdates(
    const UKControllerPlugin::Api::ApiInterface& api,
    UKControllerPlugin::Windows::WinApiInterface& windows,
    UKControllerPlugin::Curl::CurlInterface& curl
)
{
    try {
        const nlohmann::json versionDetails = UKControllerPlugin::GetUpdateData(api);
        if (UpdateRequired(windows, versionDetails)) {
            PerformUpdates(curl, windows, versionDetails);
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
    UKControllerPlugin::Curl::CurlInterface& curl,
    UKControllerPlugin::Windows::WinApiInterface& windows,
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

bool UpdateRequired(UKControllerPlugin::Windows::WinApiInterface& windows, const nlohmann::json& versionDetails)
{
    return !windows.FileExists(GetVersionLockfileLocation()) ||
        windows.ReadFromFile(GetVersionLockfileLocation()) != GetVersionFromJson(versionDetails);
}

void MoveOldUpdaterBinary(UKControllerPlugin::Windows::WinApiInterface& windows)
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

std::wstring GetOldUpdaterLocation()
{
    return GetBinariesFolderRelativePath() + L"/UKControllerPluginUpdater.dll.old";
}

void UpdateLockfile(UKControllerPlugin::Windows::WinApiInterface& windows, std::string version)
{
    windows.WriteToFile(GetVersionLockfileLocation(), version, true);
}
