#include "HoldConfigurationMenuItem.h"
#include "HoldDisplayManager.h"
#include "HoldRenderer.h"
#include "HoldSelectionMenu.h"
#include "dialog/DialogManager.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "euroscope/UserSetting.h"

using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Hold::HoldRenderer;
using UKControllerPlugin::Hold::HoldSelectionMenu;
using UKControllerPlugin::Plugin::PopupMenuItem;

namespace UKControllerPlugin {
    namespace Hold {

        HoldConfigurationMenuItem::HoldConfigurationMenuItem(
            const UKControllerPlugin::Dialog::DialogManager& dialogManager,
            const std::shared_ptr<HoldDisplayManager> displayManager,
            const unsigned int selectorMenuOpenCallbackId)
            : selectorMenuOpenCallbackId(selectorMenuOpenCallbackId), displayManager(displayManager),
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
    } // namespace Hold
} // namespace UKControllerPlugin
