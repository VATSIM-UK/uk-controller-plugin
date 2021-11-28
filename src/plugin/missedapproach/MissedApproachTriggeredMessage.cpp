#include "MissedApproachTriggeredMessage.h"
#include "time/ParseTimeStrings.h"

namespace UKControllerPlugin::MissedApproach {

    MissedApproachTriggeredMessage::MissedApproachTriggeredMessage(
        std::string callsign, bool triggeredByUser, std::chrono::system_clock::time_point expiresAt)
        : callsign(std::move(callsign)), triggeredByUser(triggeredByUser), expiresAt(expiresAt)
    {
    }

    auto MissedApproachTriggeredMessage::GetMessageData() const -> nlohmann::json
    {
        return {
            {"callsign", this->callsign},
            {"triggered_by_user", this->triggeredByUser},
            {"expires_at", Time::ToDateTimeString(this->expiresAt)},
        };
    }

    auto MissedApproachTriggeredMessage::GetMessageType() const -> Integration::MessageType
    {
        return {"missed_approach_triggered", 1};
    }
} // namespace UKControllerPlugin::MissedApproach
