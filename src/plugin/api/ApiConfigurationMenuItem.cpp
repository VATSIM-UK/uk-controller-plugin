#include "ApiConfigurationMenuItem.h"
#include "api/LocateApiSettings.h"

using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::Windows::WinApiInterface;

namespace UKControllerPlugin::Api {
    ApiConfigurationMenuItem::ApiConfigurationMenuItem(WinApiInterface& winApi, int menuCallbackId)
        : menuCallbackId(menuCallbackId), winApi(winApi)
    {
    }

    void ApiConfigurationMenuItem::Configure(int functionId, std::string subject, RECT area)
    {
        UserRequestedKeyUpdate(this->winApi);
    }

    /*
        Return the menu item to be displayed
    */
    auto ApiConfigurationMenuItem::GetConfigurationMenuItem() const -> UKControllerPlugin::Plugin::PopupMenuItem
    {
        PopupMenuItem returnVal;
        returnVal.firstValue = this->itemDescription;
        returnVal.secondValue = "";
        returnVal.callbackFunctionId = this->menuCallbackId;
        returnVal.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
        returnVal.disabled = false;
        returnVal.fixedPosition = false;
        return returnVal;
    }
} // namespace UKControllerPlugin::Api
