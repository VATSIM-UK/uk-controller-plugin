#include "pch/stdafx.h"
#include "hold/HoldConfigurationMenuItem.h"
#include "hold/HoldProfileManager.h"
#include "hold/HoldRenderer.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "euroscope/UserSetting.h"
#include "dialog/DialogManager.h"

using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;
using UKControllerPlugin::Hold::HoldProfileManager;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::Hold::HoldRenderer;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Dialog::DialogManager;

namespace UKControllerPlugin {
    namespace Hold {

        HoldConfigurationMenuItem::HoldConfigurationMenuItem(
            const DialogManager & dialogManager,
            const HoldProfileManager & profileManager, 
            HoldRenderer & renderer,
            const unsigned int selectorMenuOpenCallbackId
        ) : profileManager(profileManager), selectorMenuOpenCallbackId(selectorMenuOpenCallbackId),
            renderer(renderer), dialogManager(dialogManager)
        {

        }

        /*
            The user has selected to open the hold configuration dialog,
            open it.
        */
        void HoldConfigurationMenuItem::Configure(int functionId, std::string subject, RECT screenObjectArea)
        {
            this->dialogManager.OpenDialog(HOLD_SELECTOR_DIALOG, reinterpret_cast<LPARAM>(this));
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

        /*
            An item has been selected from the menu, load that profile
        */
        void HoldConfigurationMenuItem::SelectProfile(unsigned int profileId)
        {
            this->renderer.LoadProfile(profileId);
        }

        /*
            A hold profile has become invalidated, make sure that the renderer
            re-loads it.
        */
        void HoldConfigurationMenuItem::InvalidateProfile(unsigned int profileId)
        {
            this->renderer.LoadProfile(profileId);
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
