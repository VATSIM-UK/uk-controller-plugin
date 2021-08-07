#include "pch/pch.h"
#include "prenote/PrenoteMessage.h"
namespace UKControllerPlugin::Prenote {
    PrenoteMessage::PrenoteMessage(
            int id,
            std::string callsign,
            std::string departureAirfield,
            std::string sid,
            std::string destinationAirfield,
            int sendingControllerId,
            int targetControllerId,
            std::chrono::system_clock::time_point expiresAt
            ) : id(id), callsign(callsign), departureAirfield(departureAirfield), sid(sid),
            destinationAirfield(destinationAirfield), sendingControllerId(sendingControllerId),
            targetControllerId(targetControllerId), expiresAt(expiresAt)
    {}
    
    int PrenoteMessage::GetId() const
    {
        return id;
    }
    
    const std::string& PrenoteMessage::GetCallsign() const
    {
        return callsign;
    }
    
    const std::string& PrenoteMessage::GetDepartureAirfield() const
    {
        return departureAirfield;
    }
    
    const std::string& PrenoteMessage::GetSid() const
    {
        return sid;
    }
    
    const std::string& PrenoteMessage::GetDestinationAirfield() const
    {
        return destinationAirfield;
    }
    
    int PrenoteMessage::GetSendingControllerId() const
    {
        return sendingControllerId;
    }
    
    int PrenoteMessage::GetTargetControllerId() const
    {
        return targetControllerId;
    }
    
    const std::chrono::time_point& PrenoteMessage::GetExpiresAt() const
    {
        return expiresAt;
    }
} // namespace UKControllerPlugin::Prenote
