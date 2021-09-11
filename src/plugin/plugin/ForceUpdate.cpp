#include "ForceUpdate.h"
#include "data/PluginDataLocations.h"

using UKControllerPlugin::Windows::WinApiInterface;

namespace UKControllerPlugin::Plugin {

    ForceUpdate::ForceUpdate(WinApiInterface& winApi, int menuCallbackId)
        : menuCallbackId(menuCallbackId), winApi(winApi)
    {
    }

    void ForceUpdate::Configure(int functionId, std::string subject, RECT screenObjectArea)
    {
        this->ForcePluginUpdate();
    }

    auto ForceUpdate::GetConfigurationMenuItem() const -> PopupMenuItem
    {
        PopupMenuItem returnVal;
        returnVal.firstValue = this->menuItemDescription;
        returnVal.secondValue = "";
        returnVal.callbackFunctionId = this->menuCallbackId;
        returnVal.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
        returnVal.disabled = false;
        returnVal.fixedPosition = false;
        return returnVal;
    }

    auto ForceUpdate::ProcessCommand(std::string command) -> bool
    {
        if (command != this->command) {
            return false;
        }

        this->ForcePluginUpdate();
        return true;
    }

    void ForceUpdate::ForcePluginUpdate() const
    {
        int response = this->winApi.OpenMessageBox(
            L"Are you sure you want to force the UK Controller Plugin to update?",
            L"UKCP Forced Update",
            MB_YESNO | MB_ICONEXCLAMATION);

        if (response != IDYES) {
            return;
        }

        // Move the updater binary
        if (this->winApi.FileExists(GetUpdaterBinaryRelativePath())) {
            this->winApi.MoveFileToNewLocation(GetUpdaterBinaryRelativePath(), GetOldUpdaterBinaryRelativePath());
        }

        // Move the core binary
        if (this->winApi.FileExists(GetCoreBinaryRelativePath())) {
            this->winApi.MoveFileToNewLocation(GetCoreBinaryRelativePath(), GetOldCoreBinaryRelativePath());
        }

        std::wstring message = L"The plugin will force an update next time it is loaded, ";
        message += L"please close all EuroScope instances and re-open.";
        this->winApi.OpenMessageBox(message.c_str(), L"UKCP Update Information", MB_OK | MB_ICONINFORMATION);
    }
} // namespace UKControllerPlugin::Plugin
