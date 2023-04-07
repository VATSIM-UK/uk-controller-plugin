#include "SendPrenoteMenu.h"
#include "PrenoteMessage.h"
#include "PrenoteMessageCollection.h"
#include "api/ApiException.h"
#include "api/ApiInterface.h"
#include "controller/ActiveCallsign.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionCollection.h"
#include "euroscope/EuroScopeCControllerInterface.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "task/TaskRunnerInterface.h"
#include "time/SystemClock.h"

namespace UKControllerPlugin::Prenote {

    SendPrenoteMenu::SendPrenoteMenu(
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

    void SendPrenoteMenu::DisplayControllerSelectionMenu(
        Euroscope::EuroScopeCFlightPlanInterface& flightplan, const POINT& mousePos)
    {
        if (!this->activeCallsigns.UserHasCallsign() ||
            !this->activeCallsigns.GetUserCallsign().GetNormalisedPosition().SendsPrenoteMessages()) {
            return;
        }

        auto userPosition = this->activeCallsigns.GetUserCallsign().GetNormalisedPosition();
        bool menuInitialised = false;
        this->plugin.ApplyFunctionToAllControllers(
            [this, &menuInitialised, &mousePos, &flightplan, &userPosition](
                const std::shared_ptr<Euroscope::EuroScopeCControllerInterface>& controller) {
                if (controller->IsCurrentUser() || !this->activeCallsigns.CallsignActive(controller->GetCallsign())) {
                    return;
                }

                auto normalisedPosition =
                    this->activeCallsigns.GetCallsign(controller->GetCallsign()).GetNormalisedPosition();

                // If the position can't receive prenotes or one's already on the go, don't include
                if (!normalisedPosition.ReceivesPrenoteMessages() ||
                    this->PrenoteMessageExists(flightplan, userPosition.GetId(), normalisedPosition.GetId())) {
                    return;
                }

                if (!menuInitialised) {
                    RECT popupArea = {mousePos.x, mousePos.y, mousePos.x + MENU_WIDTH, mousePos.y + MENU_HEIGHT};

                    this->plugin.TriggerPopupList(popupArea, "Send Prenote", 1);
                    menuInitialised = true;
                }

                // Add an item to the menu
                Plugin::PopupMenuItem menuItem;
                menuItem.firstValue = normalisedPosition.GetCallsign();
                menuItem.secondValue = "";
                menuItem.callbackFunctionId = this->callbackId;
                menuItem.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
                menuItem.disabled = false;
                menuItem.fixedPosition = false;
                this->plugin.AddItemToPopupList(menuItem);
            });
    }

    void SendPrenoteMenu::ControllerForPrenoteSelected(const std::string& callsign)
    {
        auto fp = this->plugin.GetSelectedFlightplan();
        if (!fp) {
            LogWarning("Tried to send prenote but flightplan was not selected");
            return;
        }

        if (!activeCallsigns.UserHasCallsign()) {
            LogWarning("Tried to send prenote but user does not have active callsign");
            return;
        }

        auto userPosition = activeCallsigns.GetUserCallsign().GetNormalisedPosition();

        auto targetController = this->controllers.FetchPositionByCallsign(callsign);
        if (!targetController) {
            LogError("Tried to create a prenote but selected controller was invalid");
            return;
        }

        auto requestingController = controllers.FetchPositionById(userPosition.GetId());

        std::string flightplanCallsign = fp->GetCallsign();
        std::string origin = fp->GetOrigin();
        std::string destination = fp->GetDestination();
        std::string sid = fp->GetSidName();

        this->taskRunner.QueueAsynchronousTask(
            [this, flightplanCallsign, origin, destination, sid, requestingController, targetController]() {
                try {
                    auto prenoteResponse = this->api.CreatePrenoteMessage(
                        flightplanCallsign,
                        origin,
                        sid,
                        destination,
                        requestingController->GetId(),
                        targetController->GetId(),
                        MESSAGE_EXPIRY_SECONDS);

                    if (!ResponseValid(prenoteResponse)) {
                        LogError("Invalid prenote creation response");
                        return;
                    }

                    auto prenoteId = prenoteResponse.at("id").get<int>();
                    this->messages->Add(std::make_shared<PrenoteMessage>(
                        prenoteId,
                        flightplanCallsign,
                        origin,
                        sid,
                        destination,
                        requestingController,
                        targetController,
                        Time::TimeNow() + std::chrono::seconds(MESSAGE_EXPIRY_SECONDS)));
                    LogInfo("Created prenote message " + std::to_string(prenoteId));
                } catch (Api::ApiException&) {
                    LogError("ApiException when creating prenote messsage");
                }
            });
    }

    auto SendPrenoteMenu::ResponseValid(nlohmann::json& response) -> bool
    {
        return response.is_object() && response.contains("id") && response.at("id").is_number_integer();
    }

    auto SendPrenoteMenu::PrenoteMessageExists(
        Euroscope::EuroScopeCFlightPlanInterface& flightplan, int sendingControllerId, int targetControllerId) const
        -> bool
    {
        return this->messages->FirstWhere(
                   [&flightplan, &targetControllerId, &sendingControllerId](
                       const std::shared_ptr<PrenoteMessage>& message) -> bool {
                       return message->GetCallsign() == flightplan.GetCallsign() &&
                              message->GetSendingControllerId() == sendingControllerId &&
                              message->GetTargetControllerId() == targetControllerId;
                   }) != nullptr;
    }
} // namespace UKControllerPlugin::Prenote
