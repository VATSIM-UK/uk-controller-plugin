#include "OpenLogsFolder.h"
#include "data/PluginDataLocations.h"
#include "log/LoggerBootstrap.h"
#include "windows/WinApiInterface.h"

namespace UKControllerPlugin::Plugin {
    OpenLogsFolder::OpenLogsFolder(const Windows::WinApiInterface& windows, int menuCallbackId)
        : windows(windows), menuCallbackId(menuCallbackId)
    {
    }

    void OpenLogsFolder::Configure(int functionId, std::string subject, RECT screenObjectArea)
    {
        windows.OpenExplorer(
            GetFullPluginDataRoot() + L"/" + UKControllerPlugin::Log::LoggerBootstrap::GetLogsFolder());
    }

    auto OpenLogsFolder::GetConfigurationMenuItem() const -> PopupMenuItem
    {
        PopupMenuItem returnVal;
        returnVal.firstValue = "Plugin Logs Folder (Opens In Explorer)";
        returnVal.secondValue = "";
        returnVal.callbackFunctionId = this->menuCallbackId;
        returnVal.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
        returnVal.disabled = false;
        returnVal.fixedPosition = false;

        return returnVal;
    }
} // namespace UKControllerPlugin::Plugin
