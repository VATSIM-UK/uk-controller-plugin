#include "PluginChangelog.h"
#include "update/LoadChangelog.h"

using UKControllerPlugin::Windows::WinApiInterface;

namespace UKControllerPlugin::Plugin {

    PluginChangelog::PluginChangelog(WinApiInterface& winApi, int menuCallbackId)
        : menuCallbackId(menuCallbackId), winApi(winApi)
    {
    }

    void PluginChangelog::Configure(int functionId, std::string subject, RECT screenObjectArea)
    {
        this->ShowChangelog();
    }

    auto PluginChangelog::GetConfigurationMenuItem() const -> PopupMenuItem
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

    auto PluginChangelog::ProcessCommand(std::string command) -> bool
    {
        if (command != this->command) {
            return false;
        }

        this->ShowChangelog();
        return true;
    }

    void PluginChangelog::ShowChangelog() const
    {
        Update::LoadChangelog(this->winApi);
    }
} // namespace UKControllerPlugin::Plugin
