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
            std::chrono::system_clock::time_point expiresAt);
        [[nodiscard]] auto GetId() const -> int;
        [[nodiscard]] auto GetCallsign() const -> const std::string&;
        [[nodiscard]] auto GetDepartureAirfield() const -> const std::string&;
        [[nodiscard]] auto GetSid() const -> const std::string&;
        [[nodiscard]] auto GetDestinationAirfield() const -> const std::string&;
        [[nodiscard]] auto GetSendingControllerId() const -> int;
        [[nodiscard]] auto GetTargetControllerId() const -> int;
        [[nodiscard]] auto GetExpiresAt() const -> const std::chrono::system_clock::time_point&;
        [[nodiscard]] auto IsAcknowledged() const -> bool;
        void Acknowledge();

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
        std::chrono::system_clock::time_point expiresAt;

        // Has it been acknowledged
        bool acknowledged;
    };
} // namespace UKControllerPlugin::Prenote
