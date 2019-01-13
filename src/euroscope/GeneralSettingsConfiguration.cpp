#include "pch/stdafx.h"
#include "euroscope/GeneralSettingsConfiguration.h"
#include "euroscope/UserSetting.h"
#include "windows/WinApiInterface.h"
#include "euroscope/GeneralSettingsDialog.h"

using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Windows::WinApiInterface;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::Euroscope::GeneralSettingsDialog;

namespace UKControllerPlugin {
    namespace Euroscope {

        GeneralSettingsConfiguration::GeneralSettingsConfiguration(
            UserSetting & userSettings,
            const WinApiInterface & winApi,
            const int menuSelectedCallbackId
        ) : userSettings(userSettings), winApi(winApi), menuSelectedCallbackId(menuSelectedCallbackId)
        {

        }

        /*
            When the user selects this in the configuration menu, we want to open the dialog box for configuration.
        */
        void GeneralSettingsConfiguration::Configure(std::string subject)
        {
            this->winApi.OpenGeneralSettingsDialog(this->userSettings);
        }

        /*
            Returns how this should be represented in a Euroscope list.
        */
        PopupMenuItem GeneralSettingsConfiguration::GetConfigurationMenuItem(void) const
        {
            PopupMenuItem returnVal;
            returnVal.firstValue = this->configMenuDescription;
            returnVal.secondValue = "";
            returnVal.callbackFunctionId = this->menuSelectedCallbackId;
            returnVal.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            returnVal.disabled = false;
            returnVal.fixedPosition = false;
            return returnVal;
        }

        /*
            If the user enters the command for opening the general settings dialog, open the dialog.
        */
        bool GeneralSettingsConfiguration::ProcessCommand(std::string command)
        {
            if (command != ".ukcp g") {
                return false;
            }

            this->winApi.OpenGeneralSettingsDialog(this->userSettings);
            return true;
        }
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
