#include "pch/pch.h"
#include "plugin/PluginChangelog.h"
#include "update/LoadChangelog.h"

using UKControllerPlugin::Windows::WinApiInterface;

namespace UKControllerPlugin {
    namespace Plugin {

        PluginChangelog::PluginChangelog(WinApiInterface& winApi, int menuCallbackId)
            : winApi(winApi), menuCallbackId(menuCallbackId)
        {
        }

        void PluginChangelog::Configure(int functionId, std::string subject, RECT screenObjectArea)
        {
            this->ShowChangelog();
        }

        PopupMenuItem PluginChangelog::GetConfigurationMenuItem(void) const
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

        bool PluginChangelog::ProcessCommand(std::string command)
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
    }  // namespace Plugin
}  // namespace UKControllerPlugin
