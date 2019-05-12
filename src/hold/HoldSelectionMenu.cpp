#include "pch/stdafx.h"
#include "hold/HoldSelectionMenu.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "hold/HoldManager.h"
#include "plugin/PopupMenuItem.h"
#include "hold/ManagedHold.h"
#include "hold/HoldProfileManager.h"
#include "hold/HoldDisplayManager.h"

using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;
using UKControllerPlugin::Hold::HoldManager;
using UKControllerPlugin::Hold::HoldProfileManager;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::Hold::ManagedHold;

namespace UKControllerPlugin {
    namespace Hold {
        HoldSelectionMenu::HoldSelectionMenu(
            HoldManager & holdManager,
            HoldProfileManager & profileManager,
            EuroscopePluginLoopbackInterface & plugin,
            unsigned int callbackIdFirstHold
        ) : holdManager(holdManager), profileManager(profileManager),
            plugin(plugin), callbackIdFirstHold(callbackIdFirstHold)
        {
        }

        /*
            Add a display manager to the set so its holds can be used in the list
        */
        void HoldSelectionMenu::AddDisplayManager(const std::shared_ptr<const HoldDisplayManager> manager)
        {
            this->displays.insert(manager);
        }

        /*
            Return the number of hold display managers.
        */
        size_t HoldSelectionMenu::CountDisplayManagers(void) const
        {
            return this->displays.size();
        }

        /*
            Display the menu
        */
        void HoldSelectionMenu::DisplayMenu(
            EuroScopeCFlightPlanInterface & flightplan,
            EuroScopeCRadarTargetInterface & radarTarget,
            std::string context,
            const POINT & mousePos
        ) {
            // Create the list in place
            RECT popupArea = {
                mousePos.x,
                mousePos.y,
                mousePos.x + 300,
                mousePos.y + 500
            };

            this->plugin.TriggerPopupList(
                popupArea,
                "Hold",
                1
            );

            // Use a "different" callback function for each hold, so we can easily determine which one is called
            PopupMenuItem menuItem;
            menuItem.secondValue = "";
            menuItem.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            menuItem.disabled = false;
            menuItem.fixedPosition = false;

            // Iterate through the profiles
            std::set<unsigned int> addedHolds;
            for (
                std::set<std::shared_ptr<const HoldDisplayManager>>::const_iterator it = this->displays.cbegin();
                it != this->displays.cend();
                ++it
            ) {
                // Add holds to the list with no duplicates
                for (
                    HoldDisplayManager::const_iterator holdIt = (*it)->cbegin();
                    holdIt != (*it)->cend();
                    ++holdIt
                ) {
                    if (!addedHolds.insert((*holdIt)->managedHold.GetHoldParameters().identifier).second) {
                        continue;
                    }

                    menuItem.firstValue = (*holdIt)->managedHold.GetHoldParameters().description;
                    menuItem.callbackFunctionId = this->callbackIdFirstHold +
                        (*holdIt)->managedHold.GetHoldParameters().identifier;
                    this->plugin.AddItemToPopupList(menuItem);
                }
            }

            // Add the cancel menu item.
            PopupMenuItem menuItemCancel;
            menuItemCancel.firstValue = "--";
            menuItemCancel.secondValue = "";
            menuItemCancel.callbackFunctionId = this->callbackIdFirstHold;
            menuItemCancel.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            menuItemCancel.disabled = false;
            menuItemCancel.fixedPosition = true;
            this->plugin.AddItemToPopupList(menuItemCancel);
        }

        /*
            The menu item has been clicked, add the currently selected aircraft to the
            selcted hold.
        */
        void HoldSelectionMenu::MenuItemClicked(int functionId, std::string context)
        {
            // The hold ID is the number in sequence of the callback functions
            int holdId = functionId - this->callbackIdFirstHold;
            std::shared_ptr<EuroScopeCFlightPlanInterface> fp = this->plugin.GetSelectedFlightplan();
            std::shared_ptr<EuroScopeCRadarTargetInterface> rt = this->plugin.GetSelectedRadarTarget();

            if (!fp || !rt) {
                LogWarning("Tried to put a non existent flight into a hold");
                return;
            }

            // If we click the first option, that's the cancel button
            if (functionId == this->callbackIdFirstHold) {
                this->holdManager.RemoveAircraftFromAnyHold(fp->GetCallsign());
                return;
            }

            this->holdManager.AddAircraftToHold(
                *fp,
                *rt,
                holdId
            );
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
