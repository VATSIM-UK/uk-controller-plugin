#include "pch/pch.h"
#include "hold/HoldConfigurationMenuItem.h"
#include "hold/HoldSelectionMenu.h"
#include "hold/HoldRenderer.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "euroscope/UserSetting.h"
#include "dialog/DialogManager.h"
#include "hold/HoldDisplayManager.h"

using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;
using UKControllerPlugin::Hold::HoldSelectionMenu;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::Hold::HoldRenderer;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Dialog::DialogManager;

namespace UKControllerPlugin {
    namespace Hold {

        HoldConfigurationMenuItem::HoldConfigurationMenuItem(
            const UKControllerPlugin::Dialog::DialogManager & dialogManager,
            const std::shared_ptr<HoldDisplayManager> displayManager,
            const unsigned int selectorMenuOpenCallbackId
        ) : displayManager(displayManager), selectorMenuOpenCallbackId(selectorMenuOpenCallbackId),
            dialogManager(dialogManager)
        {

        }

        /*
            The user has selected to open the hold configuration dialog,
            open it.
        */
        void HoldConfigurationMenuItem::Configure(int functionId, std::string subject, RECT screenObjectArea)
        {
            this->dialogManager.OpenDialog(IDD_HOLD_SELECTION, reinterpret_cast<LPARAM>(this));
        }

        /*
            Get the configuration menu item
        */
        UKControllerPlugin::Plugin::PopupMenuItem HoldConfigurationMenuItem::GetConfigurationMenuItem(void) const
        {
            PopupMenuItem returnVal;
            returnVal.firstValue = this->menuItemDescription;
            returnVal.secondValue = "";
            returnVal.callbackFunctionId = this->selectorMenuOpenCallbackId;
            returnVal.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            returnVal.disabled = false;
            returnVal.fixedPosition = false;
            return returnVal;
        }

        std::vector<std::string> HoldConfigurationMenuItem::GetHolds(void) const
        {
            return this->displayManager->GetSelectedHolds();
        }

        /*
            An item has been selected from the menu, load that profile
        */
        void HoldConfigurationMenuItem::SelectHolds(std::vector<std::string> holds)
        {
            this->displayManager->LoadSelectedHolds(holds);
        }

        /*
            Process the dot command
        */
        bool HoldConfigurationMenuItem::ProcessCommand(std::string command)
        {
            if (command == this->openDialogCommand) {
                this->Configure(-1, "", {});
                return true;
            }

            return false;
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
