#include "loader/pch.h"
#include "loader/loader.h"
#include "log/LoggerBootstrap.h"
#include "include/UKControllerPlugin.h"
#include "euroscope/EuroScopePlugIn.h"
#include "windows/WinApiInterface.h"
#include "windows/WinApiBootstrap.h"
#include "api/ApiBootstrap.h"
#include "setting/SettingRepositoryFactory.h"
#include "curl/CurlApi.h"
#include "data/PluginDataLocations.h"

HINSTANCE pluginDllInstance;

std::unique_ptr<UKControllerPlugin::Windows::WinApiInterface> windows;
/*
 *  Update and load the UKControllerPlugin DLL
 */
void __declspec (dllexport) EuroScopePlugInInit(EuroScopePlugIn::CPlugIn** ppPlugInInstance)
{
    // Boot up the windows API, create the root folder and create the logger
    windows = UKControllerPlugin::Windows::Bootstrap(nullptr);
    CreatePluginDataRoot(*windows);
    UKControllerPlugin::Log::LoggerBootstrap::Bootstrap(*windows, L"loader");

    // Bootstrap the API, download the updater if we don't have it already and run it
    UKControllerPlugin::Curl::CurlApi curl;
    std::unique_ptr<UKControllerPlugin::Setting::SettingRepository> settings =
        UKControllerPlugin::Setting::SettingRepositoryFactory::Create(*windows);
    std::unique_ptr<UKControllerPlugin::Api::ApiInterface> api = UKControllerPlugin::Api::Bootstrap(
        *settings,
        curl
    );

    // Perform a first time download, or run the updater
    if (!FirstTimeDownload(*api, *windows, curl)) {
        RunUpdater(*windows);
    }

    // Load the core plugin library
    pluginDllInstance = LoadPluginLibrary(*windows);
    if (pluginDllInstance == nullptr) {
        UnloadPluginLibrary(pluginDllInstance, *windows);
        std::wstring message = L"Unable to load the UKControllerPluginCore DLL.\r\n";
        message += L"Please contact the VATSIM UK Web Services Department.\r\n";

        MessageBox(GetActiveWindow(), message.c_str(), L"UKCP Bootstrap Failed", MB_OK | MB_ICONSTOP);
        throw std::exception("UKCP broke");
    }

    // Load the plugin
    *ppPlugInInstance = LoadPlugin();
};

/*
 * Unload the UKControllerPlugin DLL and free the library
 */
void __declspec (dllexport) EuroScopePlugInExit(void)
{
    UnloadPluginLibrary(pluginDllInstance, *windows);
}
