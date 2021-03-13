#include "pch.h"
#include "include/UKControllerPlugin.h"
#include "euroscope/EuroScopePlugIn.h"
#include "loader/loader.h"

HINSTANCE dllInstance;

/*
 *  Load the UKControllerPlugin DLL
 */
void __declspec (dllexport) EuroScopePlugInInit (EuroScopePlugIn :: CPlugIn ** ppPlugInInstance)
{
    #ifndef _DEBUG

        CheckForUpdates();
    #endif

    // Load the library
    dllInstance = LoadLibrary(L"UKControllerPluginLibs.dll");

    if (dllInstance == NULL)
    {
        FreeLibrary(dllInstance);
        std::wstring message = L"Unable to load the UKControllerPlugin DLL.\r\n";
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
void __declspec (dllexport) EuroScopePlugInExit (void)
{
    UnloadPlugin();
    if (dllInstance != NULL)
    {
        FreeLibrary(dllInstance);
    }
}
