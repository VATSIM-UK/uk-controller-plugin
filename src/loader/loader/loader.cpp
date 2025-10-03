#include "loader/loader.h"
#include "api/ApiInterface.h"
#include "windows/WinApiInterface.h"
#include "curl/CurlInterface.h"
#include "data/PluginDataLocations.h"
#include "update/UpdateBinaries.h"
#include "euroscope/EuroScopePlugIn.h"

// Define the functions we need from the various libraries
typedef bool(CALLBACK* PERFORMUPDATES)();
typedef EuroScopePlugIn::CPlugIn*(CALLBACK* LOADPLUGINLIBRARY)();
typedef void(CALLBACK* UNLOADPLUGINLIBRARY)();

void RunUpdater(UKControllerPlugin::Windows::WinApiInterface& windows)
{
    LogInfo("Loading updater library");
    HINSTANCE updaterHandle = windows.LoadLibraryRelative(GetUpdaterBinaryRelativePath());
    if (!updaterHandle) {
        LogInfo("Unable to run the updater, binary does not exist.");
        std::wstring message = L"Unable to start updater.\r\n";
        message += L"Please contact the VATSIM UK Web Services Department.\r\n";

        windows.OpenMessageBox(message.c_str(), L"UKCP Update Failed", MB_OK | MB_ICONSTOP);
        throw std::exception("UKCP broke");
    }

    LogInfo("Performing updates, please refer to the updater log for more information");
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wcast-function-type-mismatch"
    PERFORMUPDATES PerformUpdates =
        reinterpret_cast<PERFORMUPDATES>(windows.GetFunctionPointerFromLibrary(updaterHandle, "PerformUpdates"));
#pragma clang diagnostic pop

    /*
     * If the updater returns false, it means the user wouldn't let it update.
     * Stop them here.
     */
    if (!PerformUpdates || !PerformUpdates()) {
        windows.UnloadLibrary(updaterHandle);
        throw std::exception();
    }
    LogInfo("Updates complete");
    windows.UnloadLibrary(updaterHandle);
}

HINSTANCE LoadPluginLibrary(UKControllerPlugin::Windows::WinApiInterface& windows)
{
    LogInfo("Loading core plugin library");
    return windows.LoadLibraryRelative(GetCoreBinaryRelativePath());
}

EuroScopePlugIn::CPlugIn*
LoadPlugin(HINSTANCE pluginLibraryHandle, UKControllerPlugin::Windows::WinApiInterface& windows)
{
    if (pluginLibraryHandle == nullptr) {
        LogInfo("Not loading core plugin, library handle is null");
        return nullptr;
    }

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wcast-function-type-mismatch"
    LOADPLUGINLIBRARY LoadPlugin =
        reinterpret_cast<LOADPLUGINLIBRARY>(windows.GetFunctionPointerFromLibrary(pluginLibraryHandle, "LoadPlugin"));
#pragma clang diagnostic pop

    if (!LoadPlugin) {
        LogError("Cannot find LoadPlugin function in Core binary");
        std::wstring message = L"Unable find LoadPlugin function in Core Binary.\r\n\r\n";
        message += L"Please contact the Web Services Department.";
        DisplayLoaderError(windows, message);
        return nullptr;
    }

    LogInfo("Performing core plugin load");
    return LoadPlugin();
}

void UnloadPlugin(HINSTANCE pluginLibraryHandle, UKControllerPlugin::Windows::WinApiInterface& windows)
{
    if (pluginLibraryHandle == nullptr) {
        LogInfo("Not unloading core plugin, library handle is null");
        return;
    }

    UNLOADPLUGINLIBRARY UnloadPlugin = reinterpret_cast<UNLOADPLUGINLIBRARY>(
        windows.GetFunctionPointerFromLibrary(pluginLibraryHandle, "UnloadPlugin"));

    if (!UnloadPlugin) {
        LogError("Cannot find UnloadPlugin function in Core binary");
        std::wstring message = L"Unable find UnloadPlugin function in Core Binary.\r\n\r\n";
        message += L"Please contact the Web Services Department.";
        DisplayLoaderError(windows, message);
        return;
    }

    LogInfo("Performing core plugin shutdown");
    UnloadPlugin();
}

void UnloadPluginLibrary(HINSTANCE instance, UKControllerPlugin::Windows::WinApiInterface& windows)
{
    if (instance == nullptr) {
        LogInfo("Not unloading core library, was never loaded");
        return;
    }

    LogInfo("Unloading core library");
    windows.UnloadLibrary(instance);
}

bool FirstTimeDownload(
    const UKControllerPlugin::Api::ApiInterface& api,
    UKControllerPlugin::Windows::WinApiInterface& windows,
    UKControllerPlugin::Curl::CurlInterface& curl)
{
    if (windows.FileExists(GetUpdaterBinaryRelativePath())) {
        LogInfo("Updater binary exists in filesystem, skipping first time download");
        return true;
    }

    if (!DisplayFirstTimeDownloadMessage(windows)) {
        LogWarning("User did not consent to first-time download.");
        return false;
    }

    LogInfo("Performing first time download of updater");
    nlohmann::json updateData;
    try {
        updateData = UKControllerPlugin::GetUpdateData(api, "stable");
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

bool DisplayFirstTimeDownloadMessage(UKControllerPlugin::Windows::WinApiInterface& windows)
{
    std::wstring message = L"Performing first time download. This will download and run the UKCP Updater ";
    message += L"to install the core plugin.\r\n\r\n";
    message += L"Select OK to continue.";
    return windows.OpenMessageBox(message.c_str(), L"UKCP First Time Download", MB_OKCANCEL | MB_ICONINFORMATION) ==
           IDOK;
}

void DisplayFirstTimeDownloadFailedMessage(UKControllerPlugin::Windows::WinApiInterface& windows)
{
    std::wstring message = L"Unable to perform first time download of the UKCP updater.\r\n";
    message += L"Please contact the VATSIM UK Web Services Department.\r\n";

    DisplayLoaderError(windows, message);
}

void DisplayLoaderError(UKControllerPlugin::Windows::WinApiInterface& windows, std::wstring message)
{
    windows.OpenMessageBox(message.c_str(), L"UKCP Loader Error", MB_OK | MB_ICONSTOP);
}
