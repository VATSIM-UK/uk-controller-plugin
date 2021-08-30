#include "CancelPrenoteMessageMenu.h"
#include "PrenoteMessage.h"
#include "PrenoteMessageCollection.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"

namespace UKControllerPlugin::Prenote {
    CancelPrenoteMessageMenu::CancelPrenoteMessageMenu(
        std::shared_ptr<PrenoteMessageCollection> messages,
        const Controller::ControllerPositionCollection& controllers,
        Euroscope::EuroscopePluginLoopbackInterface& plugin
    )
        : messages(std::move(messages)), controllers(controllers), plugin(plugin)
    {
    }
    
    void CancelPrenoteMessageMenu::DisplayPrenoteToDeleteMenu(
        Euroscope::EuroScopeCFlightPlanInterface& flightplan,
        const POINT& mousePos)
    {
        bool menuInitialised = false;
        this->messages->Iterate([this, &flightplan, &mousePos, &menuInitialised](const std::shared_ptr<PrenoteMessage>&
            message) {
            if (message->GetCallsign() != flightplan.GetCallsign()) {
                return;
            }
            
            if (!menuInitialised) {
                RECT popupArea = {
                    mousePos.x,
                    mousePos.y,
                    mousePos.x + MENU_WIDTH,
                    mousePos.y + MENU_HEIGHT
                };
                
                this->plugin.TriggerPopupList(
                    popupArea,
                    "Cancel Prenote",
                    1
                );
                menuInitialised = true;
            }
            
            // Add an item to the menu
            Plugin::PopupMenuItem menuItem;
            menuItem.firstValue = this->controllers.FetchPositionById(
                                                       release.second->TargetController()
                                                           )->GetCallsign();
            menuItem.secondValue = "";
            menuItem.callbackFunctionId = this->releaseCancellationCallbackId;
            menuItem.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            menuItem.disabled = false;
            menuItem.fixedPosition = false;
            this->plugin.AddItemToPopupList(menuItem);
        });
    }
} // namespace UKControllerPlugin::Prenote
