#include "pch/stdafx.h"
#include "api/ApiConfigurationMenuItem.h"
#include "api/LocateApiSettings.h"

using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::Windows::WinApiInterface;

namespace UKControllerPlugin {
    namespace Api {
        ApiConfigurationMenuItem::ApiConfigurationMenuItem(WinApiInterface & winApi, unsigned int menuCallbackId)
            : winApi(winApi), menuCallbackId(menuCallbackId)
        {

        }

        void ApiConfigurationMenuItem::Configure(int functionId, std::string subject, RECT area)
        {
            UserRequestedKeyUpdate(this->winApi);
        }

        /*
            Return the menu item to be displayed
        */
        UKControllerPlugin::Plugin::PopupMenuItem ApiConfigurationMenuItem::GetConfigurationMenuItem(void) const
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
    }  // namespace Api
}  // namespace UKControllerPlugin
