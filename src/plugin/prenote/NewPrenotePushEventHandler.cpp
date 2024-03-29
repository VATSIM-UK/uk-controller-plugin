#include "NewPrenotePushEventHandler.h"
#include "PrenoteMessage.h"
#include "PrenoteMessageCollection.h"
#include "PrenoteMessageEventHandlerCollection.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionCollection.h"
#include "time/ParseTimeStrings.h"
#include "time/SystemClock.h"

using UKControllerPlugin::Push::PushEventSubscription;
using UKControllerPlugin::Time::ParseTimeString;
using UKControllerPlugin::Time::TimeNow;

namespace UKControllerPlugin::Prenote {
    NewPrenotePushEventHandler::NewPrenotePushEventHandler(
        std::shared_ptr<PrenoteMessageCollection> prenotes,
        const Controller::ControllerPositionCollection& controllers,
        const PrenoteMessageEventHandlerCollection& eventHandlers)
        : prenotes(std::move(prenotes)), controllers(controllers), eventHandlers(eventHandlers)
    {
    }

    void NewPrenotePushEventHandler::ProcessPushEvent(const Push::PushEvent& message)
    {
        auto messageData = message.data;
        if (!MessageValid(messageData)) {
            LogError("Invalid new prenote message");
            return;
        }

        // We might have created it, so don't duplicate it
        int prenoteId = messageData.at("id").get<int>();
        auto prenoteMessage = prenotes->GetById(prenoteId);
        if (prenotes->GetById(prenoteId) == nullptr) {
            prenoteMessage = std::make_shared<PrenoteMessage>(
                prenoteId,
                messageData.at("callsign").get<std::string>(),
                messageData.at("departure_airfield").get<std::string>(),
                messageData.at("departure_sid").is_null() ? "" : messageData.at("departure_sid").get<std::string>(),
                messageData.at("destination_airfield").is_null()
                    ? ""
                    : messageData.at("destination_airfield").get<std::string>(),
                this->controllers.FetchPositionById(messageData.at("sending_controller").get<int>()),
                this->controllers.FetchPositionById(messageData.at("target_controller").get<int>()),
                ParseTimeString(messageData.at("expires_at").get<std::string>()));
            prenotes->Add(prenoteMessage);
        }

        eventHandlers.NewMessage(*prenoteMessage);
        LogInfo("Received prenote id " + std::to_string(prenoteId));
    }

    auto NewPrenotePushEventHandler::GetPushEventSubscriptions() const -> std::set<PushEventSubscription>
    {
        return {{PushEventSubscription::SUB_TYPE_EVENT, "prenote-message.received"}};
    }

    auto NewPrenotePushEventHandler::MessageValid(const nlohmann::json& message) const -> bool
    {
        return message.contains("id") && message.at("id").is_number_integer() && message.contains("callsign") &&
               message.at("callsign").is_string() && message.contains("departure_airfield") &&
               message.at("departure_airfield").is_string() && message.contains("departure_sid") &&
               (message.at("departure_sid").is_string() || message.at("departure_sid").is_null()) &&
               message.contains("destination_airfield") &&
               (message.at("destination_airfield").is_string() || message.at("destination_airfield").is_null()) &&
               message.contains("sending_controller") && message.at("sending_controller").is_number_integer() &&
               this->controllers.FetchPositionById(message.at("sending_controller").get<int>()) != nullptr &&
               message.contains("target_controller") && message.at("target_controller").is_number_integer() &&
               this->controllers.FetchPositionById(message.at("target_controller").get<int>()) != nullptr &&
               message.contains("expires_at") && message.at("expires_at").is_string() &&
               ParseTimeString(message.at("expires_at").get<std::string>()) != Time::invalidTime;
    }
} // namespace UKControllerPlugin::Prenote
