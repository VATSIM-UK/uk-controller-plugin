#include "pch/stdafx.h"
#include "hold/HoldSelectionMenu.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "hold/HoldManager.h"
#include "plugin/PopupMenuItem.h"
#include "hold/ManagedHold.h"

using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;
using UKControllerPlugin::Hold::HoldManager;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::Hold::ManagedHold;

namespace UKControllerPlugin {
    namespace Hold {
        HoldSelectionMenu::HoldSelectionMenu(
            HoldManager & holdManager,
            EuroscopePluginLoopbackInterface & plugin,
            unsigned int callbackIdFirstHold
        )
            : holdManager(holdManager), plugin(plugin), callbackIdFirstHold(callbackIdFirstHold)
        {
        }

        /*
            Adds the given hold to the menu
        */
        void HoldSelectionMenu::AddHoldToMenu(unsigned int holdId)
        {
            this->menuHolds.insert(holdId);
        }

        size_t HoldSelectionMenu::CountHolds(void) const
        {
            return this->menuHolds.size();
        }

        /*
            Remove the given hold from the menu
        */
        void HoldSelectionMenu::RemoveHoldFromMenu(unsigned int holdId)
        {
            this->menuHolds.erase(holdId);
        }

        void HoldSelectionMenu::DisplayMenu(
            EuroScopeCFlightPlanInterface & flightplan,
            EuroScopeCRadarTargetInterface & radarTarget,
            std::string context,
            const POINT & mousePos
        ) {
            if (this->menuHolds.size() == 0) {
                return;
            }

            // Create the list in place
            RECT popupArea = {
                mousePos.x,
                mousePos.y,
                mousePos.x + 300,
                mousePos.y + 500
            };

            this->plugin.TriggerPopupList(
                popupArea,
                "Hold Selection For " + flightplan.GetCallsign(),
                1
            );

            // Add items to the list
            for (
                std::set<unsigned int>::const_iterator it = this->menuHolds.cbegin();
                it != this->menuHolds.cend();
                ++it
            ) {
                const ManagedHold * hold = this->holdManager.GetManagedHold(*it);
                if (!hold) {
                    LogWarning("Tried to add invalid hold to hold selection: " + std::to_string(*it));
                    continue;
                }

                // Use a "different" callback function for each hold, so we can easily determine which one is called
                PopupMenuItem menuItem;
                menuItem.firstValue = hold->holdParameters.description;
                menuItem.secondValue = "";
                menuItem.callbackFunctionId = this->callbackIdFirstHold + *it;
                menuItem.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
                menuItem.disabled = false;
                menuItem.fixedPosition = false;

                this->plugin.AddItemToPopupList(menuItem);
            }

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
