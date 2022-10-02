#include "stands/StandUnassignedMessage.h"

namespace UKControllerPlugin::Stands {

    StandUnassignedMessage::StandUnassignedMessage(std::string callsign)
        : callsign(callsign)
    {

    }

    nlohmann::json StandUnassignedMessage::GetMessageData() const
    {
        return nlohmann::json {
            {"callsign", this->callsign}
        };
    }

    Integration::MessageType StandUnassignedMessage::GetMessageType() const
    {
        return {
            "stand_unassigned",
            1
        };
    }
} // namespace UKControllerPlugin::Stands
