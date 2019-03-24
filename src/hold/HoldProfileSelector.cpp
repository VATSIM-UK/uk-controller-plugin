#include "pch/stdafx.h"
#include "hold/HoldProfileSelector.h"
#include "hold/HoldProfileManager.h"
#include "hold/HoldRenderer.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"

using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;
using UKControllerPlugin::Hold::HoldProfileManager;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::Hold::HoldRenderer;

namespace UKControllerPlugin {
    namespace Hold {

        HoldProfileSelector::HoldProfileSelector(
            const HoldProfileManager & profileManager, 
            EuroscopePluginLoopbackInterface & plugin,
            HoldRenderer & renderer,
            const unsigned int selectorMenuOpenCallbackId,
            unsigned int firstProfileCallbackId
        ) : profileManager(profileManager), plugin(plugin), selectorMenuOpenCallbackId(selectorMenuOpenCallbackId),
            firstProfileCallbackId(firstProfileCallbackId), renderer(renderer)
        {

        }

        /*
            The user has selected to open the menu, load the menu and populate with profiles
        */
        void HoldProfileSelector::Configure(int functionId, std::string subject, RECT screenObjectArea)
        {
            RECT menuArea = {
                screenObjectArea.left - 50,
                screenObjectArea.top - 50,
                screenObjectArea.left + 10,
                screenObjectArea.top + 10
            };
            this->plugin.TriggerPopupList(menuArea, "Select Hold Profile", 2);

            PopupMenuItem menuItem;
            menuItem.firstValue = "";
            menuItem.secondValue = "--";
            menuItem.callbackFunctionId = this->firstProfileCallbackId;
            menuItem.checked = this->selectedProfile == 0 ? true : false;
            menuItem.disabled = false;
            menuItem.fixedPosition = false;

            this->plugin.AddItemToPopupList(menuItem);

            for (
                HoldProfileManager::HoldProfiles::const_iterator it = this->profileManager.cbegin();
                it != this->profileManager.cend();
                ++it
            ) {
                menuItem.firstValue = std::to_string(it->id);
                menuItem.secondValue = it->name;
                menuItem.checked = this->selectedProfile == it->id ? true : false;
            }
        }

        /*
            Get the configuration menu item
        */
        UKControllerPlugin::Plugin::PopupMenuItem HoldProfileSelector::GetConfigurationMenuItem(void) const
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
        void HoldProfileSelector::ItemSelected(int itemId, std::string itemText)
        {
            if (itemText == this->noProfileFirstColumn) {
                this->renderer.LoadProfile(this->renderer.unloadAllProfileId);
                return;
            }

            this->renderer.LoadProfile(std::stoi(itemText));
            this->selectedProfile = std::stoi(itemText);
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
