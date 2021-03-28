#include "pch.h"
#include "loader/loader.h"
#include "helper/HelperFunctions.h"
#include "api/ApiInterface.h"
#include "windows/WinApiInterface.h"
#include "data/PluginDataLocations.h"

using UKControllerPlugin::HelperFunctions;


void RunUpdater(
    const UKControllerPlugin::Windows::WinApiInterface& windows
)
{
    windows.LoadLibraryRelative(GetUpdaterBinaryRelativePath());
}

HINSTANCE LoadPlugin(
    const UKControllerPlugin::Windows::WinApiInterface& windows
)
{
    return windows.LoadLibraryRelative(GetCoreBinaryRelativePath());
}

void UnloadPlugin(
    HINSTANCE instance,
    const UKControllerPlugin::Windows::WinApiInterface& windows
)
{
    if (instance == nullptr) {
        return;
    }

    windows.UnloadLibrary(instance);
}

void FirstTimeDownloadUpdater(
    const UKControllerPlugin::Api::ApiInterface& api,
    const UKControllerPlugin::Windows::WinApiInterface& windows
)
{ }
