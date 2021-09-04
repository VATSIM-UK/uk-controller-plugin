#include "AcknowledgePrenoteMessage.h"
#include "PrenoteMessage.h"
#include "PrenoteMessageCollection.h"
#include "api/ApiException.h"
#include "api/ApiInterface.h"
#include "controller/ActiveCallsign.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPosition.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "task/TaskRunnerInterface.h"

namespace UKControllerPlugin::Prenote {
    AcknowledgePrenoteMessage::AcknowledgePrenoteMessage(
        std::shared_ptr<PrenoteMessageCollection> messages,
        const Controller::ActiveCallsignCollection& activeCallsigns,
        TaskManager::TaskRunnerInterface& taskRunner,
        const Api::ApiInterface& api)
        : messages(std::move(messages)), activeCallsigns(activeCallsigns), taskRunner(taskRunner), api(api)
    {
    }

    void AcknowledgePrenoteMessage::Acknowledge(Euroscope::EuroScopeCFlightPlanInterface& flightplan)
    {
        if (!activeCallsigns.UserHasCallsign()) {
            return;
        }

        auto userPositionId = activeCallsigns.GetUserCallsign().GetNormalisedPosition().GetId();
        auto message = this->messages->FirstWhere(
            [userPositionId, &flightplan](const std::shared_ptr<PrenoteMessage>& message) -> bool {
                return message->GetCallsign() == flightplan.GetCallsign() &&
                       message->GetTargetControllerId() == userPositionId;
            });

        if (message == nullptr) {
            return;
        }

        this->taskRunner.QueueAsynchronousTask([this, message] {
            try {
                this->api.AcknowledgePrenoteMessage(message->GetId(), message->GetTargetControllerId());
                message->Acknowledge();
                LogInfo("Acknowleded prenote message " + std::to_string(message->GetId()));
            } catch (Api::ApiException&) {
                LogError("ApiException when trying to acknowledge prenote message");
            }
        });
    }
} // namespace UKControllerPlugin::Prenote
