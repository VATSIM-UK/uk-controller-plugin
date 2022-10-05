#include "PrenoteAcknowledgedPushEventHandler.h"
#include "PrenoteMessage.h"
#include "PrenoteMessageCollection.h"
#include "PrenoteMessageEventHandlerCollection.h"
#include "push/PushEventSubscription.h"

using UKControllerPlugin::Push::PushEventSubscription;

namespace UKControllerPlugin::Prenote {
    PrenoteAcknowledgedPushEventHandler::PrenoteAcknowledgedPushEventHandler(
        std::shared_ptr<PrenoteMessageCollection> prenotes, const PrenoteMessageEventHandlerCollection& eventHandlers)
        : prenotes(std::move(prenotes)), eventHandlers(eventHandlers)
    {
    }

    void PrenoteAcknowledgedPushEventHandler::ProcessPushEvent(const Push::PushEvent& message)
    {
        auto messageData = message.data;
        if (!MessageValid(messageData)) {
            LogError("Invalid acknowledge prenote message");
            return;
        }

        int prenoteId = messageData.at("id").get<int>();
        const auto prenote = prenotes->GetById(prenoteId);
        prenote->Acknowledge();
        eventHandlers.MessageAcknowledged(*prenote);
        LogInfo("Acknowledged prenote id " + std::to_string(prenoteId));
    }

    auto PrenoteAcknowledgedPushEventHandler::GetPushEventSubscriptions() const -> std::set<PushEventSubscription>
    {
        return {{PushEventSubscription::SUB_TYPE_EVENT, "prenote-message.acknowledged"}};
    }

    auto PrenoteAcknowledgedPushEventHandler::MessageValid(const nlohmann::json& message) const -> bool
    {
        return message.contains("id") && message.at("id").is_number_integer() &&
               this->prenotes->GetById(message.at("id").get<int>()) != nullptr;
    }
} // namespace UKControllerPlugin::Prenote
