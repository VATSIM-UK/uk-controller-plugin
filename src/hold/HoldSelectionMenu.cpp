#include "pch/stdafx.h"
#include "hold/HoldSelectionMenu.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "hold/HoldManager.h"
#include "plugin/PopupMenuItem.h"
#include "hold/HoldDisplayManager.h"

using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;
using UKControllerPlugin::Hold::HoldManager;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::TaskManager::TaskRunnerInterface;

namespace UKControllerPlugin {
    namespace Hold {
        HoldSelectionMenu::HoldSelectionMenu(
            HoldManager & holdManager,
            EuroscopePluginLoopbackInterface & plugin,
            unsigned int callbackId
        ) : holdManager(holdManager), plugin(plugin), callbackId(callbackId)
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
            menuItem.firstValue = this->noHold;
            menuItem.secondValue = "";
            menuItem.callbackFunctionId = this->callbackId;
            menuItem.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            menuItem.disabled = false;
            menuItem.fixedPosition = false;

            // Iterate through the displays
            std::set<std::string> addedHolds;
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
                    if (!addedHolds.insert((*holdIt)->navaid.identifier).second) {
                        continue;
                    }

                    menuItem.firstValue = (*holdIt)->navaid.identifier;
                    this->plugin.AddItemToPopupList(menuItem);
                }
            }

            // Add the final item, no hold
            menuItem.firstValue = this->noHold;
            menuItem.fixedPosition = true;
            this->plugin.AddItemToPopupList(menuItem);
        }

        /*
            The menu item has been clicked, add the currently selected aircraft to the
            selcted hold.
        */
        void HoldSelectionMenu::MenuItemClicked(int functionId, std::string context)
        {
            std::shared_ptr<EuroScopeCFlightPlanInterface> fp = this->plugin.GetSelectedFlightplan();

            if (!fp) {
                LogWarning("Tried to put a non existent flight into a hold");
                return;
            }

            // Only allow this if aircraft is tracked.
            if (!fp->IsTrackedByUser()) {
                LogInfo("Attempted to assign hold but flightplan is not tracked by user " + fp->GetCallsign());
                return;
            }

            if (context == "--") {
                // Dont do anything if already not holding
                std::shared_ptr<HoldingAircraft> holdingAircraft = this->holdManager.GetHoldingAircraft(
                    fp->GetCallsign()
                );
                if (
                    holdingAircraft == nullptr ||
                    holdingAircraft->GetAssignedHold() == holdingAircraft->noHoldAssigned
                ) {
                    return;
                }

                this->holdManager.UnassignAircraftFromHold(fp->GetCallsign(), true);
            } else {
                // Dont do anything if aircraft already holding here
                std::shared_ptr<HoldingAircraft> holdingAircraft = this->holdManager.GetHoldingAircraft(
                    fp->GetCallsign()
                );
                if (holdingAircraft != nullptr && holdingAircraft->GetAssignedHold() == context) {
                    return;
                }
                this->holdManager.AssignAircraftToHold(fp->GetCallsign(), context, true);
            }
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
