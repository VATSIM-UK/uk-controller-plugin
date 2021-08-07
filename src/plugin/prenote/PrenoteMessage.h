#pragma once

namespace UKControllerPlugin::Prenote {
    /**
     * Represents a single prenote messsage.
     */
    class PrenoteMessage
    {
        public:
            PrenoteMessage(
                int id,
                std::string callsign,
                std::string departureAirfield,
                std::string sid,
                std::string destinationAirfield,
                int sendingControllerId,
                int targetControllerId,
                std::chrono::system_clock::time_point expiresAt
            );
            int GetId() const;
            const std::string& GetCallsign() const;
            const std::string& GetDepartureAirfield() const;
            const std::string& GetSid() const;
            const std::string& GetDestinationAirfield() const;
            int GetSendingControllerId() const;
            int GetTargetControllerId() const;
            const std::chrono::system_clock::time_point& GetExpiresAt() const;
    
        private:
            // The prenote message id
            int id;
            
            // The callsign
            std::string callsign;
            
            // The departure airfield
            std::string departureAirfield;
            
            // The departure SID
            std::string sid;
            
            // The destination airfield
            std::string destinationAirfield;
            
            // Who sent the prenote
            int sendingControllerId;
            
            // Who the prenote was aimed at
            int targetControllerId;
            
            // What time the prenote expires
            std::chrono::system_clock::time_point expiresAt
    };
} // namespace UKControllerPlugin::Prenote
