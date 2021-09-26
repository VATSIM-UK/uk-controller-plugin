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
        if (!this->MessageValid(data)) {
            LogError("Invalid missed approach push event");
            return;
        }

        if (CheckForExisting(data.at("callsign").get<std::string>())) {
            return;
        }

        this->missedApproaches->Add(std::make_shared<class MissedApproach>(
            data.at("callsign").get<std::string>(), Time::ParseTimeString(data.at("expires_at").get<std::string>())));
    }

    auto NewMissedApproachPushEventHandler::GetPushEventSubscriptions() const -> std::set<PushEventSubscription>
    {
        return {{PushEventSubscription::SUB_TYPE_EVENT, "missed-approach.created"}};
    }

    auto NewMissedApproachPushEventHandler::MessageValid(const nlohmann::json& data) -> bool
    {
        return data.contains("callsign") && data.at("callsign").is_string() && data.contains("expires_at") &&
               data.at("expires_at").is_string() &&
               Time::ParseTimeString(data.at("expires_at").get<std::string>()) != Time::invalidTime;
    }

    auto NewMissedApproachPushEventHandler::CheckForExisting(std::string callsign) const -> bool
    {
        auto existing = this->missedApproaches->FirstWhere(
            [&callsign](const std::shared_ptr<class MissedApproach>& missed) -> bool {
                return missed->Callsign() == callsign;
            });

        // If one has expired, we can remove it and pretend it was never there
        if (existing && existing->IsExpired()) {
            this->missedApproaches->Remove(existing);
            return false;
        }

        return existing != nullptr;
    }
} // namespace UKControllerPlugin::MissedApproach
