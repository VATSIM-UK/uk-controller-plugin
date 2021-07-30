#include "pch/stdafx.h"
#include "plugin/PluginHelpPage.h"

using UKControllerPlugin::Windows::WinApiInterface;

namespace UKControllerPlugin {
    namespace Plugin {

        PluginHelpPage::PluginHelpPage(WinApiInterface& winApi, int menuCallbackId)
            : winApi(winApi), menuCallbackId(menuCallbackId)
        {
        }

        void PluginHelpPage::Configure(int functionId, std::string subject, RECT screenObjectArea)
        {
            this->ShowHelpPage();
        }

        PopupMenuItem PluginHelpPage::GetConfigurationMenuItem(void) const
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

        bool PluginHelpPage::ProcessCommand(std::string command)
        {
            if (command != this->command) {
                return false;
            }

            this->ShowHelpPage();
            return true;
        }

        void PluginHelpPage::ShowHelpPage()
        {
            this->winApi.OpenWebBrowser(this->helpUrl);
        }
    }  // namespace Plugin
}  // namespace UKControllerPlugin
