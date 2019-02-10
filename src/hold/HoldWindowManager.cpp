#include "pch/stdafx.h"
#include "hold/HoldWindowManager.h"
#include "hold/HoldDisplay.h"
#include "hold/HoldManager.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "plugin/PopupMenuItem.h"

using UKControllerPlugin::Hold::HoldManager;
using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Plugin::PopupMenuItem;

namespace UKControllerPlugin {
    namespace Hold {

        HoldWindowManager::HoldWindowManager(
            HWND euroscopeWindow,
            HINSTANCE dllInstance,
            const HoldManager & holdManager,
            EuroscopePluginLoopbackInterface & plugin
        )
            : euroscopeWindow(euroscopeWindow), dllInstance(dllInstance), holdManager(holdManager), plugin(plugin)
        {

        }

        void HoldWindowManager::AddWindow(void)
        {
            this->holdDisplays.emplace_back(euroscopeWindow, dllInstance, *this->holdManager.GetManagedHold(1));
        }

        size_t HoldWindowManager::CountDisplays(void) const
        {
            return this->holdDisplays.size();
        }

        /*
            Open up a popup menu displaying the available holds to add to
        */
        void HoldWindowManager::OpenHoldPopupMenu(
            EuroScopeCFlightPlanInterface & flightplan,
            EuroScopeCRadarTargetInterface & radarTarget,
            std::string context,
            const POINT & mousePos
        ) {
            if (this->holdDisplays.size() == 0) {
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
                "Hold Selection",
                1
            );

            // Add items to the list
            for (
                std::list<UKControllerPlugin::Hold::HoldDisplay>::const_iterator it = this->holdDisplays.cbegin();
                it != this->holdDisplays.cend();
                ++it
            ) {
                
                std::string menuItemText = it->managedHold.holdParameters.description + "("
                    + std::to_string(it->managedHold.holdParameters.identifier) + ")";

                PopupMenuItem menuItem;
                menuItem.firstValue = menuItemText;
                menuItem.secondValue = "";
                menuItem.callbackFunctionId = this->itemClickedFunctionId;
                menuItem.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
                menuItem.disabled = false;
                menuItem.fixedPosition = false;

                this->plugin.AddItemToPopupList(menuItem);
            }
        }

    }  // namespace Hold
}  // namespace UKControllerPlugin
