#include "pch/stdafx.h"
#include "hold/HoldEventHandler.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope//EuroScopeCRadarTargetInterface.h"
#include "hold/HoldManager.h"
#include "plugin/PopupMenuItem.h"

using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;
using UKControllerPlugin::TimedEvent::AbstractTimedEvent;
using UKControllerPlugin::Hold::HoldWindowManager;
using UKControllerPlugin::Hold::HoldManager;
using UKControllerPlugin::Plugin::PopupMenuItem;

namespace UKControllerPlugin {
    namespace Hold {

        HoldEventHandler::HoldEventHandler(
            HoldManager & holdManager,
            EuroscopePluginLoopbackInterface & plugin,
            UKControllerPlugin::Hold::HoldWindowManager & holdManagerWindow,
            const int popupMenuItemId
        )
            : holdManager(holdManager), plugin(plugin), holdManagerWindow(std::move(holdManagerWindow)),
            popupMenuItemId(popupMenuItemId)
        {

        }

        void HoldEventHandler::FlightPlanEvent(
            EuroScopeCFlightPlanInterface & flightPlan,
            EuroScopeCRadarTargetInterface & radarTarget
        ) {
            // Nothing to do here
        }

        /*
            When a flightplan disconnects, remove that aircraft from the hold.
        */
        void HoldEventHandler::FlightPlanDisconnectEvent(EuroScopeCFlightPlanInterface & flightPlan)
        {
            this->holdManager.RemoveAircraftFromAnyHold(flightPlan.GetCallsign());
        }

        void HoldEventHandler::ControllerFlightPlanDataEvent(
            EuroScopeCFlightPlanInterface & flightPlan,
            int dataType
        ) {
            // Nothing to do here
        }

        /*
            When the timed event is called, we should update the holding aircrafts details
        */
        void HoldEventHandler::TimedEventTrigger(void)
        {
            this->holdManager.UpdateHoldingAircraft(this->plugin);
        }

        /*
            Called when the popup menu item is selected. Open the hold manager window.
        */
        void HoldEventHandler::Configure(std::string subject)
        {
            this->holdManagerWindow.AddWindow();
        }

        /*
            Return the item for the popup configuration menu.
        */
        UKControllerPlugin::Plugin::PopupMenuItem HoldEventHandler::GetConfigurationMenuItem(void) const
        {
            PopupMenuItem item;
            item.callbackFunctionId = this->popupMenuItemId;
            item.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            item.disabled = false;
            item.firstValue = this->menuItemDescription;
            item.secondValue = "";
            item.fixedPosition = false;
            return item;
        }

        /*
            Process euroscope dot commands
        */
        bool HoldEventHandler::ProcessCommand(std::string command)
        {
            if (command != this->toggleCommand) {
                return false;
            }

            this->holdManagerWindow.AddWindow();
            return true;
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
