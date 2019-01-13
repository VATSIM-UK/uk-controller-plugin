#include "pch/stdafx.h"
#include "bootstrap/InitialisePlugin.h"

// The one true app to rule them all...
UKControllerPlugin::InitialisePlugin thePluginApp;

/*
    Called by Euroscope when the plugin is loaded, either on startup (if previously loaded and saved
    in settings) or when manually loaded by the user.
*/
void __declspec(dllexport) EuroScopePlugInInit(EuroScopePlugIn::CPlugIn ** ppPlugInInstance)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // Give ES the plugin instance and run the post initialisation method.
    thePluginApp.PostInit();
    *ppPlugInInstance = thePluginApp.GetPlugin();
}


/*
    Called by Euroscope when the plugin is unloaded, either by the user or on exit.
*/
void __declspec(dllexport) EuroScopePlugInExit(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    thePluginApp.EuroScopeCleanup();
}
