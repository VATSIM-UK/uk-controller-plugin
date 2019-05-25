#include "pch/stdafx.h"
#include "euroscope/GeneralSettingsConfiguration.h"
#include "dialog/DialogManager.h"
#include "euroscope/GeneralSettingsDialog.h"

using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::Euroscope::GeneralSettingsDialog;

namespace UKControllerPlugin {
    namespace Euroscope {

        GeneralSettingsConfiguration::GeneralSettingsConfiguration(
            const DialogManager & dialogManager,
            const int menuSelectedCallbackId
        ) : dialogManager(dialogManager), menuSelectedCallbackId(menuSelectedCallbackId)
        {

        }

        /*
            When the user selects this in the configuration menu, we want to open the dialog box for configuration.
        */
        void GeneralSettingsConfiguration::Configure(int functionId, std::string subject, RECT screenObjectArea)
        {
            this->dialogManager.OpenDialog(this->dialogId, NULL);
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

            this->dialogManager.OpenDialog(this->dialogId, NULL);
            return true;
        }
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
