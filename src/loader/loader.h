#pragma once

// Forward declarations
namespace UKControllerPlugin
{
    namespace Api
    {
        class ApiInterface;
    } // namespace Api

    namespace Windows
    {
        class WinApiInterface;
    } // namespace Windows

    namespace Curl {
        class CurlInterface;
    } // namespace Curl
}

void RunUpdater(UKControllerPlugin::Windows::WinApiInterface& windows);
HINSTANCE LoadPluginLibrary(const UKControllerPlugin::Windows::WinApiInterface& windows);
void UnloadPluginLibrary(HINSTANCE instance, const UKControllerPlugin::Windows::WinApiInterface& windows);
bool FirstTimeDownload(
    const UKControllerPlugin::Api::ApiInterface& api,
    UKControllerPlugin::Windows::WinApiInterface& windows,
    UKControllerPlugin::Curl::CurlInterface& curl
);
