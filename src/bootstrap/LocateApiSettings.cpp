#include "pch/stdafx.h"
#include "bootstrap/LocateApiSettings.h"
#include "windows/WinApiInterface.h"
#include "setting/SettingRepository.h"

using UKControllerPlugin::Windows::WinApiInterface;
using UKControllerPlugin::Setting::SettingRepository;

namespace UKControllerPlugin {
namespace Bootstrap {

    void LocateApiSettings(WinApiInterface & winApi, SettingRepository & settings)
    {
        if (settings.HasSetting("api-url") && settings.HasSetting("api-key")) {
            return;
        }

        winApi.OpenMessageBox(
            L"No valid user API configuration found, please select the file to use",
            L"UKCP Message",
            MB_OK | MB_ICONINFORMATION
        );

        ReplaceApiSettings(winApi);

        // Load the settings into the repo
        settings.AddSettingsFromJsonFile("api-settings.json");
    }

    /*
        Replace the WinApi settings
    */
    void ReplaceApiSettings(WinApiInterface & winApi)
    {
        // Select the file to get settings from
        COMDLG_FILTERSPEC fileTypes[] =
        {
            { L"JSON", L"*.json" },
        };
        std::string apiSettings = winApi.ReadFromFile(
            winApi.FileOpenDialog(
                L"Select API Settings File",
                1,
                fileTypes
            ),
            false
        );

        // Write the selected file to disk
        winApi.WriteToFile("settings/api-settings.json", apiSettings, true);
    }

    /*
        The user has requested an update to an API key
    */
    void UserRequestedKeyUpdate(WinApiInterface & winApi)
    {
        winApi.OpenMessageBox(
            L"Please select the settings file to use as your new key",
            L"UKCP Message",
            MB_OK | MB_ICONINFORMATION
        );
        ReplaceApiSettings(winApi);
        winApi.OpenMessageBox(
            L"Settings file has been replaced. Please restart EuroScope for the changes to take effect.",
            L"UKCP Message",
            MB_OK | MB_ICONINFORMATION
        );
    }

}  // namespace Bootstrap
}  // namespace UKControllerPlugin
