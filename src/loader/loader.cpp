#include "loader/pch.h"
#include "loader/loader.h"
#include "api/ApiInterface.h"
#include "windows/WinApiInterface.h"
#include "curl/CurlInterface.h"
#include "data/PluginDataLocations.h"
#include "update/UpdateBinaries.h"
#include "updater/UKControllerPluginUpdater.h"


void RunUpdater(
    UKControllerPlugin::Windows::WinApiInterface& windows
)
{
    HINSTANCE updaterHandle = windows.LoadLibraryRelative(GetUpdaterBinaryRelativePath());

    if (!updaterHandle) {
        std::wstring message = L"Unable to start updater.\r\n";
        message += L"Please contact the VATSIM UK Web Services Department.\r\n";

        windows.OpenMessageBox(message.c_str(), L"UKCP Update Failed", MB_OK | MB_ICONSTOP);
        throw std::exception("UKCP broke");
    }

    PerformUpdates();
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
        return false;
    }

    nlohmann::json updateData;
    try {
        updateData = UKControllerPlugin::GetUpdateData(api);
    } catch (std::exception) {
        DisplayFirstTimeDownloadFailedMessage(windows);
        return false;
    }

    if (!UKControllerPlugin::DownloadUpdater(updateData, windows, curl)) {
        DisplayFirstTimeDownloadFailedMessage(windows);
        return false;
    }

    return true;
}

void DisplayFirstTimeDownloadFailedMessage(UKControllerPlugin::Windows::WinApiInterface& windows)
{
    std::wstring message = L"Unable to perform first time download of the UKCP updater.\r\n";
    message += L"Please contact the VATSIM UK Web Services Department.\r\n";

    windows.OpenMessageBox(message.c_str(), L"UKCP Update Failed", MB_OK | MB_ICONSTOP);
}
