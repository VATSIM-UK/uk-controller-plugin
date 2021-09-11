#include "PluginHelpPage.h"

using UKControllerPlugin::Windows::WinApiInterface;

namespace UKControllerPlugin::Plugin {

    PluginHelpPage::PluginHelpPage(WinApiInterface& winApi, int menuCallbackId)
        : menuCallbackId(menuCallbackId), winApi(winApi)
    {
    }

    void PluginHelpPage::Configure(int functionId, std::string subject, RECT screenObjectArea)
    {
        this->ShowHelpPage();
    }

    auto PluginHelpPage::GetConfigurationMenuItem() const -> PopupMenuItem
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

    auto PluginHelpPage::ProcessCommand(std::string command) -> bool
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
} // namespace UKControllerPlugin::Plugin
