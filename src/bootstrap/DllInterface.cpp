#include "pch/stdafx.h"
#include "bootstrap/InitialisePlugin.h"
#include "update/PluginVersion.h"

#ifndef UKCP_CORE_API
#define UKCP_CORE_API extern "C" __declspec(dllexport)
#define UKCP_CORE_DIRECT_API __declspec(dllexport)
#endif


using UKControllerPlugin::Plugin::PluginVersion;

// The one true app to rule them all...
UKControllerPlugin::InitialisePlugin thePluginApp;

HINSTANCE dllInstance;

/*
    The DLL entry point
*/
BOOL WINAPI DllMain(
    HINSTANCE hinstance,
    DWORD dwReason,
    LPVOID lpvReserved
) {
    dllInstance = hinstance;
    return TRUE;
}

/*
    Called by EuroScope when the plugin is loaded, either on startup (if previously loaded and saved
    in settings) or when manually loaded by the user.
*/
UKCP_CORE_API EuroScopePlugIn::CPlugIn* LoadPlugin(void)
{
    // Give ES the plugin instance and run the post initialisation method.
    INITCOMMONCONTROLSEX common = {sizeof(INITCOMMONCONTROLSEX), ICC_LINK_CLASS};
    HRESULT test1 = InitCommonControlsEx(&common);
    try {
        thePluginApp.PostInit(dllInstance);
        return thePluginApp.GetPlugin();
    }
    catch (std::exception e) {
        std::string what = e.what();
        std::string typeId = typeid(e).name();


        std::wstring message = L"Exception thrown when bootstrapping UKCP.\r\n";
        message += L"Please contact the VATSIM UK Web Services Department.\r\n";
        message += L"Type: " + std::wstring(typeId.cbegin(), typeId.cend()) + L"\r\n";
        message += L"Message: " + std::wstring(what.cbegin(), what.cend()) + L"\r\n";;
        message += L"Errno: " + std::to_wstring(errno) + L"\r\n";;
        message += L"LastError: " + std::to_wstring(GetLastError()) + L"\r\n";;

        MessageBox(GetActiveWindow(), message.c_str(), L"UKCP Bootstrap Failed", MB_OK | MB_ICONSTOP);
    }

    return nullptr;
}


/*
    Called by Euroscope when the plugin is unloaded, either by the user or on exit.
*/
UKCP_CORE_API void UnloadPlugin(void)
{
    try {
        thePluginApp.EuroScopeCleanup();
    }
    catch (std::exception e) {
        std::string what = e.what();

        std::wstring message = L"Exception thrown when shutting down UKCP.\r\n";
        message += L"Please contact the VATSIM UK Web Services Department.\r\n";
        message += L"Message: " + std::wstring(what.cbegin(), what.cend());

        MessageBox(GetActiveWindow(), message.c_str(), L"UKCP Shutdown Failed", MB_OK | MB_ICONSTOP);
    }
}

#ifdef _DEBUG
/*
 *  Allows the plugin DLL to be loaded directly, so we don't have to worry about going via the loader.
 */
UKCP_CORE_DIRECT_API void EuroScopePlugInInit(EuroScopePlugIn::CPlugIn** ppPlugInInstance)
{
    *ppPlugInInstance = LoadPlugin();
};

/*
 * Unload the UKControllerPlugin DLL directly, so we don't have to worry about going via the loader.
 */
UKCP_CORE_DIRECT_API void EuroScopePlugInExit(void)
{
    UnloadPlugin();
}
#endif


