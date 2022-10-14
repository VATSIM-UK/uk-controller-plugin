#include "OpenDataFolder.h"
#include "data/PluginDataLocations.h"
#include "windows/WinApiInterface.h"

namespace UKControllerPlugin::Plugin {
    OpenDataFolder::OpenDataFolder(const Windows::WinApiInterface& windows, int menuCallbackId)
        : windows(windows), menuCallbackId(menuCallbackId)
    {
    }

    void OpenDataFolder::Configure(int functionId, std::string subject, RECT screenObjectArea)
    {
        windows.OpenExplorer(GetFullPluginDataRoot());
    }

    auto OpenDataFolder::GetConfigurationMenuItem() const -> PopupMenuItem
    {
        PopupMenuItem returnVal;
        returnVal.firstValue = "Plugin Data Folder (Opens In Explorer)";
        returnVal.secondValue = "";
        returnVal.callbackFunctionId = this->menuCallbackId;
        returnVal.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
        returnVal.disabled = false;
        returnVal.fixedPosition = false;

        return returnVal;
    }
} // namespace UKControllerPlugin::Plugin
