#include "loader/pch.h"
#include "loader/loader.h"
#include "log/LoggerBootstrap.h"
#include "euroscope/EuroScopePlugIn.h"
#include "windows/WinApiInterface.h"
#include "windows/WinApiBootstrap.h"
#include "api/ApiBootstrap.h"
#include "setting/SettingRepositoryFactory.h"
#include "curl/CurlApi.h"
#include "data/PluginDataLocations.h"

#ifndef UKCP_LOADER_API
#define UKCP_LOADER_API __declspec(dllexport)
#endif

HINSTANCE pluginDllInstance;
HINSTANCE loaderDllInstance;
std::unique_ptr<UKControllerPlugin::Windows::WinApiInterface> windows;

// Define the functions in the core DLL that we will need to call to load and unload the plugin
typedef EuroScopePlugIn::CPlugIn* (CALLBACK* LOADPLUGINLIBRARY)();
typedef void (CALLBACK* UNLOADPLUGINLIBRARY)();

BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,
    DWORD fdwReason,
    LPVOID lpReserved
)
{
    loaderDllInstance = hinstDLL;
    return TRUE;
}

/*
 *  Update and load the UKControllerPlugin DLL
 */
void UKCP_LOADER_API EuroScopePlugInInit(EuroScopePlugIn::CPlugIn** ppPlugInInstance)
{
    // Boot up the windows API, create the root folder and create the logger
    windows = UKControllerPlugin::Windows::Bootstrap(loaderDllInstance);
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

    // Perform a first time download if required, then run the updater
    if (FirstTimeDownload(*api, *windows, curl)) {
        RunUpdater(*windows);
    }

    // Load the core plugin library
    LogInfo("Loading plugin binary");
    pluginDllInstance = LoadPluginLibrary(*windows);
    if (pluginDllInstance == nullptr) {
        LogError("Failed to load plugin binary");
        UnloadPluginLibrary(pluginDllInstance, *windows);
        std::wstring message = L"Unable to load the UKControllerPluginCore DLL.\r\n";
        message += L"Please contact the VATSIM UK Web Services Department.\r\n";

        MessageBox(GetActiveWindow(), message.c_str(), L"UKCP Bootstrap Failed", MB_OK | MB_ICONSTOP);
        throw std::exception("UKCP broke");
    }

    // Load the plugin
    LOADPLUGINLIBRARY LoadPlugin = reinterpret_cast<LOADPLUGINLIBRARY>(GetProcAddress(pluginDllInstance, "LoadPlugin"));
    *ppPlugInInstance = LoadPlugin();
    LogInfo("Plugin binary loaded successfully");
};

/*
 * Unload the UKControllerPlugin DLL and free the library
 */
void UKCP_LOADER_API EuroScopePlugInExit(void)
{
    UNLOADPLUGINLIBRARY UnloadPlugin = reinterpret_cast<UNLOADPLUGINLIBRARY>(
        GetProcAddress(pluginDllInstance, "UnloadPlugin")
    );
    UnloadPlugin();
    UnloadPluginLibrary(pluginDllInstance, *windows);
}
