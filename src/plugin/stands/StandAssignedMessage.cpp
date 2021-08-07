#include "pch/pch.h"
#include "stands/StandAssignedMessage.h"

namespace UKControllerPlugin::Stands {
    
    StandAssignedMessage::StandAssignedMessage(std::string callsign, std::string airfield, std::string stand)
        : callsign(callsign), airfield(airfield), stand(stand)
    {
    
    }
    
    nlohmann::json StandAssignedMessage::GetMessageData() const
    {
        return nlohmann::json {
            {"callsign", this->callsign},
            {"airfield", this->airfield},
            {"stand", this->stand},
        };
    }
    
    Integration::MessageType StandAssignedMessage::GetMessageType() const
    {
        return {
            "stand_assigned",
            1
        };
    }
} // namespace UKControllerPlugin::Stands
