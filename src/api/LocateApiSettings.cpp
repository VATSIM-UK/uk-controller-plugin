#include "utils/pch.h"
#include "api/LocateApiSettings.h"
#include "windows/WinApiInterface.h"
#include "setting/SettingRepository.h"

using UKControllerPlugin::Windows::WinApiInterface;
using UKControllerPlugin::Setting::SettingRepository;

namespace UKControllerPlugin {
    namespace Api
    {

    void LocateApiSettings(WinApiInterface & winApi, SettingRepository & settings)
    {
        if (settings.HasSetting("api-url") && settings.HasSetting("api-key")) {
            LogInfo("Api configuration has been loaded");
            return;
        }

        LogInfo("First time key update");
        winApi.OpenMessageBox(
            L"No valid user API configuration found, please select the file to use",
            L"UKCP Message",
            MB_OK | MB_ICONINFORMATION
        );

        if (!ReplaceApiSettings(winApi)) {
            LogInfo("First time key update cancelled.");
            return;
        }

        // Load the settings into the repo
        settings.AddSettingsFromJsonFile("api-settings.json", true);
        LogInfo("Loaded new user API settings into repository");
    }

    /*
        Update the user key file with no prompts - for when Auth fails and we prompt them to reenter.
    */
    bool UserRequestedKeyUpdateNoPrompts(WinApiInterface & winApi, SettingRepository & settings)
    {
        if (!ReplaceApiSettings(winApi)) {
            LogInfo("Failed auth update cancelled");
            return false;
        }

        settings.AddSettingsFromJsonFile("api-settings.json", true);
        LogInfo("Loaded new user API settings into repository");
        return true;
    }

    /*
        Replace the WinApi settings
    */
    bool ReplaceApiSettings(WinApiInterface & winApi)
    {
        // Select the file to get settings from
        COMDLG_FILTERSPEC fileTypes[] =
        {
            { L"JSON", L"*.json" },
        };

        std::wstring filePath = winApi.FileOpenDialog(
            L"Select API Settings File",
            1,
            fileTypes
        );

        if (filePath == L"") {
            LogInfo("User did not select a valid key file");
            return false;
        }

        std::string apiSettings = winApi.ReadFromFile(filePath, false);

        // Write the selected file to disk
        winApi.WriteToFile(L"settings/api-settings.json", apiSettings, true);
        LogInfo("Updated user key file");
        return true;
    }

    /*
        The user has requested an update to an API key
    */
    void UserRequestedKeyUpdate(WinApiInterface & winApi)
    {
        // Confirm that they understand the consequences
        LogInfo("User requested API keyfile update");
        const int retVal = winApi.OpenMessageBox(
            L"Please select the key file to use, this will overwrite your previous key.",
            L"UKCP Message",
            MB_OKCANCEL | MB_ICONINFORMATION
        );

        if (retVal != IDOK) {
            LogInfo("User cancelled key file update");
            return;
        }

        // Do the replacement procedure.
        if (!ReplaceApiSettings(winApi)) {
            LogInfo("User requested key update cancelled");
            return;
        }

        winApi.OpenMessageBox(
            L"Settings file has been replaced. Please restart EuroScope for the changes to take effect.",
            L"UKCP Message",
            MB_OK | MB_ICONINFORMATION
        );
    }
    } // namespace Api
}  // namespace UKControllerPlugin
