#include "MissedApproach.h"
#include "MissedApproachCollection.h"
#include "NewMissedApproachPushEventHandler.h"
#include "time/ParseTimeStrings.h"

using UKControllerPlugin::MissedApproach::MissedApproach;
using UKControllerPlugin::Push::PushEventSubscription;

namespace UKControllerPlugin::MissedApproach {
    NewMissedApproachPushEventHandler::NewMissedApproachPushEventHandler(
        std::shared_ptr<MissedApproachCollection> missedApproaches)
        : missedApproaches(std::move(missedApproaches))
    {
    }

    void NewMissedApproachPushEventHandler::ProcessPushEvent(const Push::PushEvent& message)
    {
        auto data = message.data;
        if (!MessageValid(data)) {
            LogError("Invalid missed approach push event");
            return;
        }

        if (CheckForExisting(data.at("id").get<int>())) {
            return;
        }

        // If we get to this point, the missed approach wasn't user-triggered
        this->missedApproaches->Add(std::make_shared<class MissedApproach>(
            data.at("id").get<int>(),
            data.at("callsign").get<std::string>(),
            Time::ParseTimeString(data.at("expires_at").get<std::string>()),
            false));
    }

    auto NewMissedApproachPushEventHandler::GetPushEventSubscriptions() const -> std::set<PushEventSubscription>
    {
        return {{PushEventSubscription::SUB_TYPE_EVENT, "missed-approach.created"}};
    }

    auto NewMissedApproachPushEventHandler::MessageValid(const nlohmann::json& data) -> bool
    {
        return data.is_object() && data.contains("id") && data.at("id").is_number_integer() &&
               data.contains("callsign") && data.at("callsign").is_string() && data.contains("expires_at") &&
               data.at("expires_at").is_string() &&
               Time::ParseTimeString(data.at("expires_at").get<std::string>()) != Time::invalidTime;
    }

    auto NewMissedApproachPushEventHandler::CheckForExisting(int id) const -> bool
    {
        return this->missedApproaches->Get(id) != nullptr;
    }
} // namespace UKControllerPlugin::MissedApproach
