#include "bootstrap/InitialisePlugin.h"
#include "update/CheckDevelopmentVersion.h"
#include "update/PluginVersion.h"

#ifndef UKCP_CORE_API                                  // NOLINT
#define UKCP_CORE_API extern "C" __declspec(dllexport) // NOLINT
#define UKCP_CORE_DIRECT_API __declspec(dllexport)     // NOLINT
#endif                                                 // NOLINT

using UKControllerPlugin::Plugin::PluginVersion;
using UKControllerPluginUtils::Update::IsDevelopmentVersion;

// The one true app to rule them all...
UKControllerPlugin::InitialisePlugin thePluginApp; // NOLINT

HINSTANCE dllInstance; // NOLINT

/*
    The DLL entry point
*/
[[maybe_unused]] auto WINAPI
DllMain(HINSTANCE hinstance, [[maybe_unused]] DWORD dwReason, [[maybe_unused]] LPVOID lpvReserved) -> BOOL
{
    dllInstance = hinstance;
    return TRUE;
}

/*
    Called by EuroScope when the plugin is loaded, either on startup (if previously loaded and saved
    in settings) or when manually loaded by the user.
*/
UKCP_CORE_API auto LoadPlugin() -> EuroScopePlugIn::CPlugIn*
{
    // Give ES the plugin instance and run the post initialisation method.
    INITCOMMONCONTROLSEX common;
    common.dwSize = sizeof(INITCOMMONCONTROLSEX);
    common.dwICC = ICC_LINK_CLASS;
    InitCommonControlsEx(&common);
    try {
        thePluginApp.PostInit(dllInstance);
        return thePluginApp.GetPlugin();
    } catch (std::exception& e) {
        std::string what = e.what();
        std::string typeId = typeid(e).name();

        std::wstring message = L"Exception thrown when bootstrapping UKCP.\r\n";
        message += L"Please contact the VATSIM UK Web Services Department.\r\n";
        message += L"Type: " + std::wstring(typeId.cbegin(), typeId.cend()) + L"\r\n";
        message += L"Message: " + std::wstring(what.cbegin(), what.cend()) + L"\r\n";
        ;
        message += L"Errno: " + std::to_wstring(errno) + L"\r\n";
        ;
        message += L"LastError: " + std::to_wstring(GetLastError()) + L"\r\n";
        ;

        MessageBox(GetActiveWindow(), message.c_str(), L"UKCP Bootstrap Failed", MB_OK | MB_ICONSTOP);
        ShutdownLogger();
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
    } catch (std::exception& e) {
        std::string what = e.what();

        std::wstring message = L"Exception thrown when shutting down UKCP.\r\n";
        message += L"Please contact the VATSIM UK Web Services Department.\r\n";
        message += L"Message: " + std::wstring(what.cbegin(), what.cend());

        MessageBox(GetActiveWindow(), message.c_str(), L"UKCP Shutdown Failed", MB_OK | MB_ICONSTOP);
        ShutdownLogger();
    }
}

/*
 * Returns the version of the plugin in use
 */
UKCP_CORE_API auto GetPluginVersion() -> const char*
{
    return PluginVersion::version;
}

/*
 *  Allows the plugin DLL to be loaded directly, so we don't have to worry about going via the loader.
 */
UKCP_CORE_DIRECT_API void EuroScopePlugInInit(EuroScopePlugIn::CPlugIn** ppPlugInInstance)
{
    if (!IsDevelopmentVersion(PluginVersion::version)) {
        MessageBox(
            GetActiveWindow(),
            L"Core binary cannot be loaded directly in release versions.",
            L"UKCP Load Failed",
            MB_OK | MB_ICONSTOP);
        return;
    }

    *ppPlugInInstance = LoadPlugin();
};

/*
 * Unload the UKControllerPlugin DLL directly, so we don't have to worry about going via the loader.
 */
UKCP_CORE_DIRECT_API void EuroScopePlugInExit()
{
    if (!IsDevelopmentVersion(PluginVersion::version)) {
        return;
    }

    UnloadPlugin();
}
