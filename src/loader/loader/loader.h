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
} // namespace UKControllerPlugin

namespace EuroScopePlugIn {
    class CPlugIn;
} // namespace EuroScopePlugIn

void RunUpdater(UKControllerPlugin::Windows::WinApiInterface& windows);
HINSTANCE LoadPluginLibrary(UKControllerPlugin::Windows::WinApiInterface& windows);
EuroScopePlugIn::CPlugIn* LoadPlugin(
    HINSTANCE pluginLibraryHandle,
    UKControllerPlugin::Windows::WinApiInterface& windows
);
void UnloadPlugin(HINSTANCE pluginLibraryHandle, UKControllerPlugin::Windows::WinApiInterface& windows);
void UnloadPluginLibrary(HINSTANCE instance, UKControllerPlugin::Windows::WinApiInterface& windows);
bool FirstTimeDownload(
    const UKControllerPlugin::Api::ApiInterface& api,
    UKControllerPlugin::Windows::WinApiInterface& windows,
    UKControllerPlugin::Curl::CurlInterface& curl
);
bool DisplayFirstTimeDownloadMessage(UKControllerPlugin::Windows::WinApiInterface& windows);
void DisplayFirstTimeDownloadFailedMessage(UKControllerPlugin::Windows::WinApiInterface& windows);
void DisplayLoaderError(UKControllerPlugin::Windows::WinApiInterface& windows, std::wstring message);
