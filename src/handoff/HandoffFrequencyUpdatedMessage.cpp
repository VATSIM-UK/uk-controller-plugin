#include "pch/stdafx.h"
#include "handoff/HandoffFrequencyUpdatedMessage.h"

namespace UKControllerPlugin::Handoff {
    HandoffFrequencyUpdatedMessage::HandoffFrequencyUpdatedMessage(std::string callsign, std::string frequency):
        callsign(std::move(callsign)), frequency(std::move(frequency)) {}

    nlohmann::json HandoffFrequencyUpdatedMessage::GetMessageData() const
    {
        return {
            {"callsign", this->callsign},
            {"frequency", this->frequency},
        };
    }

    Integration::MessageType HandoffFrequencyUpdatedMessage::GetMessageType() const
    {
        return {
            "departure_frequency_updated",
            1
        };
    }
} // namespace UKControllerPlugin::Handoff
