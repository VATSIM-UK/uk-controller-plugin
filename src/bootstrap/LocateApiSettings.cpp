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

        // Load the settings into the repo
        settings.AddSettingsFromJsonFile("api-settings.json");
    }
}  // namespace Bootstrap
}  // namespace UKControllerPlugin
