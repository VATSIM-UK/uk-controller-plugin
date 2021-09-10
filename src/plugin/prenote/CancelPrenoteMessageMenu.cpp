#include "CancelPrenoteMessageMenu.h"
#include "PrenoteMessage.h"
#include "PrenoteMessageCollection.h"
#include "api/ApiException.h"
#include "api/ApiInterface.h"
#include "controller/ActiveCallsign.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionCollection.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "task/TaskRunnerInterface.h"

namespace UKControllerPlugin::Prenote {
    CancelPrenoteMessageMenu::CancelPrenoteMessageMenu(
        std::shared_ptr<PrenoteMessageCollection> messages,
        const Controller::ControllerPositionCollection& controllers,
        const Controller::ActiveCallsignCollection& activeCallsigns,
        Euroscope::EuroscopePluginLoopbackInterface& plugin,
        TaskManager::TaskRunnerInterface& taskRunner,
        const Api::ApiInterface& api,
        int callbackId)
        : messages(std::move(messages)), controllers(controllers), activeCallsigns(activeCallsigns), plugin(plugin),
          taskRunner(taskRunner), api(api), callbackId(callbackId)
    {
    }

    void CancelPrenoteMessageMenu::DisplayPrenoteToDeleteMenu(
        Euroscope::EuroScopeCFlightPlanInterface& flightplan, const POINT& mousePos)
    {
        if (!activeCallsigns.UserHasCallsign()) {
            return;
        }

        auto userPosition = activeCallsigns.GetUserCallsign().GetNormalisedPosition();
        if (!userPosition.SendsPrenoteMessages()) {
            return;
        }

        bool menuInitialised = false;
        this->messages->Iterate([this, &flightplan, &mousePos, &menuInitialised, &userPosition](
                                    const std::shared_ptr<PrenoteMessage>& message) {
            if (message->GetCallsign() != flightplan.GetCallsign() ||
                message->GetSendingControllerId() != userPosition.GetId()) {
                return;
            }

            if (!menuInitialised) {
                RECT popupArea = {mousePos.x, mousePos.y, mousePos.x + MENU_WIDTH, mousePos.y + MENU_HEIGHT};

                this->plugin.TriggerPopupList(popupArea, "Cancel Prenote", 1);
                menuInitialised = true;
            }

            // Add an item to the menu
            Plugin::PopupMenuItem menuItem;
            menuItem.firstValue = this->controllers.FetchPositionById(message->GetTargetControllerId())->GetCallsign();
            menuItem.secondValue = "";
            menuItem.callbackFunctionId = this->callbackId;
            menuItem.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            menuItem.disabled = false;
            menuItem.fixedPosition = false;
            this->plugin.AddItemToPopupList(menuItem);
        });
    }

    void CancelPrenoteMessageMenu::ControllerForPrenoteDeletionSelected(std::string callsign)
    {
        auto fp = this->plugin.GetSelectedFlightplan();
        if (!fp) {
            LogWarning("Tried to cancel prenote but flightplan was not selected");
            return;
        }

        if (!activeCallsigns.UserHasCallsign()) {
            LogWarning("Tried to cancel prenote but user does not have active callsign");
            return;
        }

        auto userPosition = activeCallsigns.GetUserCallsign().GetNormalisedPosition();

        try {
            auto controllerId = this->controllers.FetchPositionByCallsign(std::move(callsign)).GetId();
            auto message = this->messages->FirstWhere(
                [fp, controllerId, &userPosition](const std::shared_ptr<PrenoteMessage>& message) -> bool {
                    return message->GetCallsign() == fp->GetCallsign() &&
                           message->GetSendingControllerId() == userPosition.GetId() &&
                           message->GetTargetControllerId() == controllerId;
                });

            if (!message) {
                LogWarning("Unable to find prenote to cancel");
                return;
            }
            auto messageId = message->GetId();

            this->taskRunner.QueueAsynchronousTask([this, messageId]() {
                try {
                    this->api.DeletePrenoteMessage(messageId);
                    this->messages->Remove(messageId);
                    LogInfo("Deleted prenote message " + std::to_string(messageId));
                } catch (Api::ApiException&) {
                    LogError("ApiException when deleting prenote messsage");
                }
            });
        } catch (std::out_of_range&) {
            LogError("Tried to cancel a prenote but selected controller was invalid");
        }
    }
} // namespace UKControllerPlugin::Prenote
