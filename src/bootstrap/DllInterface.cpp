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
    try {
        thePluginApp.PostInit(dllInstance);
        *ppPlugInInstance = thePluginApp.GetPlugin();
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
        throw e;
    }
}


/*
    Called by Euroscope when the plugin is unloaded, either by the user or on exit.
*/
void __declspec(dllexport) EuroScopePlugInExit(void)
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
        throw e;
    }
}
