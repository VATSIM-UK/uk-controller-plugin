#include "FirstTimeApiConfigLoader.h"
#include "api/ApiSettingsProviderInterface.h"
#include "dialog/DialogManager.h"
#include "windows/WinApiInterface.h"

namespace UKControllerPlugin::Api {

    auto LocateConfig(
        const Dialog::DialogManager& dialogManager,
        Windows::WinApiInterface& windows,
        UKControllerPluginUtils::Api::ApiSettingsProviderInterface& settingsProvider) -> bool
    {
        if (settingsProvider.Has()) {
            LogInfo("Api configuration successfully loaded");
            return true;
        }

        std::wstring message = L"Your web browser will now open and you will be asked to log into the UK "
                               "Controller Plugin website to set up your API credentials.\n\n";
        message += L"These credentials allow the plugin to access various data sources and perform certain actions "
                   "such as assign squawk codes to aircraft.\n\n";
        message += L"Skipping this step will mean that many core features of the UK Controller Plugin will not work "
                   "as expected.";
        windows.OpenMessageBox(message.c_str(), L"UK Controller Plugin First Time Setup", MB_OK);
        bool apiKeyReplaced = false;
        dialogManager.OpenDialog(IDD_API_KEY_REPLACE, reinterpret_cast<LPARAM>(&apiKeyReplaced));
        if (!apiKeyReplaced) {
            LogInfo("First time api config load, user elected not to load config");
            return false;
        };

        LogInfo("First time api config load completed");
        return true;
    }
} // namespace UKControllerPlugin::Api
