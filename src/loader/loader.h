#pragma once

// Forward declarations
namespace UKControllerPlugin
{
    namespace Api
    {
        class ApiInterface;
    }

    namespace Windows
    {
        class WinApiInterface;
    }
}

void RunUpdater(const UKControllerPlugin::Windows::WinApiInterface& windows);
HINSTANCE LoadPluginLibrary(const UKControllerPlugin::Windows::WinApiInterface& windows);
void UnloadPluginLibrary(HINSTANCE instance, const UKControllerPlugin::Windows::WinApiInterface& windows);
void FirstTimeDownloadUpdater(
    const UKControllerPlugin::Api::ApiInterface& api,
    const UKControllerPlugin::Windows::WinApiInterface& windows
);
