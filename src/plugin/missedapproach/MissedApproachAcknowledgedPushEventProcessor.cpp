#include "MissedApproach.h"
#include "MissedApproachAcknowledgedPushEventProcessor.h"
#include "MissedApproachAcknowledgedUserMessage.h"
#include "MissedApproachCollection.h"
#include "message/UserMessager.h"

using UKControllerPlugin::Push::PushEventSubscription;

namespace UKControllerPlugin::MissedApproach {
    MissedApproachAcknowledgedPushEventProcessor::MissedApproachAcknowledgedPushEventProcessor(
        const MissedApproachCollection& missedApproaches, Message::UserMessager& messager)
        : missedApproaches(std::move(missedApproaches)), messager(messager)
    {
    }

    void MissedApproachAcknowledgedPushEventProcessor::ProcessPushEvent(const Push::PushEvent& message)
    {
        if (!MessageValid(message.data)) {
            LogWarning("Invalid missed approach acknowledgement message");
            return;
        }

        const auto approach = this->missedApproaches.Get(message.data.at("id").get<int>());
        if (!approach) {
            LogWarning("Tried acknowleding invalid missed approach");
            return;
        }

        approach->Acknowledge(
            message.data.at("acknowledged_by").get<std::string>(), message.data.at("remarks").get<std::string>());

        if (approach->CreatedByUser()) {
            messager.SendMessageToUser(MissedApproachAcknowledgedUserMessage(*approach));
        }
    }

    auto MissedApproachAcknowledgedPushEventProcessor::GetPushEventSubscriptions() const
        -> std::set<Push::PushEventSubscription>
    {
        return {{PushEventSubscription::SUB_TYPE_EVENT, "missed-approach.acknowledged"}};
    }

    auto MissedApproachAcknowledgedPushEventProcessor::MessageValid(const nlohmann::json& message) -> bool
    {
        return message.is_object() && message.contains("id") && message.at("id").is_number_integer() &&
               message.contains("acknowledged_by") && message.at("acknowledged_by").is_string() &&
               message.contains("remarks") && message.at("remarks").is_string();
    }
} // namespace UKControllerPlugin::MissedApproach
