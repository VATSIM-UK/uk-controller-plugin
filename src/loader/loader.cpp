#include "loader/pch.h"
#include "loader/loader.h"
#include "api/ApiInterface.h"
#include "windows/WinApiInterface.h"
#include "curl/CurlInterface.h"
#include "data/PluginDataLocations.h"
#include "update/UpdateBinaries.h"

// Define the function we need to run from the updater library
typedef void (CALLBACK* PERFORMUPDATES)();

void RunUpdater(
    UKControllerPlugin::Windows::WinApiInterface& windows
)
{
    HINSTANCE updaterHandle = windows.LoadLibraryRelative(GetUpdaterBinaryRelativePath());
    DWORD lastError = GetLastError();
    if (!updaterHandle) {
        LogInfo("Unable to run the updater, binary does not exist");
        std::wstring message = L"Unable to start updater.\r\n";
        message += L"Please contact the VATSIM UK Web Services Department.\r\n";

        windows.OpenMessageBox(message.c_str(), L"UKCP Update Failed", MB_OK | MB_ICONSTOP);
        throw std::exception("UKCP broke");
    }

    LogInfo("Performing updates, please refer to the updater log for more information");
    PERFORMUPDATES PerformUpdates = reinterpret_cast<PERFORMUPDATES>(GetProcAddress(updaterHandle, "PerformUpdates"));
    PerformUpdates();
    LogInfo("Updates complete");
    windows.UnloadLibrary(updaterHandle);
}

HINSTANCE LoadPluginLibrary(
    const UKControllerPlugin::Windows::WinApiInterface& windows
)
{
    return windows.LoadLibraryRelative(GetCoreBinaryRelativePath());
}

void UnloadPluginLibrary(
    HINSTANCE instance,
    const UKControllerPlugin::Windows::WinApiInterface& windows
)
{
    if (instance == nullptr) {
        return;
    }

    windows.UnloadLibrary(instance);
}

bool FirstTimeDownload(
    const UKControllerPlugin::Api::ApiInterface& api,
    UKControllerPlugin::Windows::WinApiInterface& windows,
    UKControllerPlugin::Curl::CurlInterface& curl
)
{
    if (windows.FileExists(GetUpdaterBinaryRelativePath())) {
        LogInfo("Updater binary exists in filesystem, skipping first time download");
        return true;
    }

    LogInfo("Performing first time download of updater");
    nlohmann::json updateData;
    try {
        updateData = UKControllerPlugin::GetUpdateData(api);
    } catch (std::exception) {
        LogError("Failed to get first time update data");
        DisplayFirstTimeDownloadFailedMessage(windows);
        return false;
    }

    if (!UKControllerPlugin::DownloadUpdater(updateData, windows, curl)) {
        LogError("Failed to perform first time updater download");
        DisplayFirstTimeDownloadFailedMessage(windows);
        return false;
    }

    LogInfo("First time download of updater successful");
    return true;
}

void DisplayFirstTimeDownloadFailedMessage(UKControllerPlugin::Windows::WinApiInterface& windows)
{
    std::wstring message = L"Unable to perform first time download of the UKCP updater.\r\n";
    message += L"Please contact the VATSIM UK Web Services Department.\r\n";

    windows.OpenMessageBox(message.c_str(), L"UKCP Update Failed", MB_OK | MB_ICONSTOP);
}
