#include "PrenoteDeletedPushEventHandler.h"
#include "PrenoteMessage.h"
#include "PrenoteMessageCollection.h"
#include "PrenoteMessageEventHandlerCollection.h"

using UKControllerPlugin::Push::PushEventSubscription;

namespace UKControllerPlugin::Prenote {
    PrenoteDeletedPushEventHandler::PrenoteDeletedPushEventHandler(
        std::shared_ptr<PrenoteMessageCollection> prenotes, const PrenoteMessageEventHandlerCollection& eventHandlers)
        : prenotes(std::move(prenotes)), eventHandlers(eventHandlers)
    {
    }

    void PrenoteDeletedPushEventHandler::ProcessPushEvent(const Push::PushEvent& message)
    {
        auto messageData = message.data;
        if (!MessageValid(messageData)) {
            LogError("Invalid delete prenote message");
            return;
        }

        int prenoteId = messageData.at("id").get<int>();
        eventHandlers.MessageCancelled(*prenotes->GetById(prenoteId));
        prenotes->Remove(prenoteId);
        LogInfo("Deleted prenote id " + std::to_string(prenoteId));
    }

    auto PrenoteDeletedPushEventHandler::GetPushEventSubscriptions() const -> std::set<PushEventSubscription>
    {
        return {{PushEventSubscription::SUB_TYPE_EVENT, "prenote-message.deleted"}};
    }

    auto PrenoteDeletedPushEventHandler::MessageValid(const nlohmann::json& message) const -> bool
    {
        return message.contains("id") && message.at("id").is_number_integer() &&
               this->prenotes->GetById(message.at("id").get<int>()) != nullptr;
    }
} // namespace UKControllerPlugin::Prenote
