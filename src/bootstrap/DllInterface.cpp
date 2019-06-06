#include "pch/stdafx.h"
#include "bootstrap/InitialisePlugin.h"

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
    Called by Euroscope when the plugin is loaded, either on startup (if previously loaded and saved
    in settings) or when manually loaded by the user.
*/
void __declspec(dllexport) EuroScopePlugInInit(EuroScopePlugIn::CPlugIn ** ppPlugInInstance)
{
    // Give ES the plugin instance and run the post initialisation method.
    thePluginApp.PostInit(dllInstance);
    *ppPlugInInstance = thePluginApp.GetPlugin();
}


/*
    Called by Euroscope when the plugin is unloaded, either by the user or on exit.
*/
void __declspec(dllexport) EuroScopePlugInExit(void)
{
    thePluginApp.EuroScopeCleanup();
}
