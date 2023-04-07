#pragma once

namespace UKControllerPlugin::Controller {
    class ControllerPosition;
} // namespace UKControllerPlugin::Controller

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
            std::shared_ptr<Controller::ControllerPosition> sendingController,
            std::shared_ptr<Controller::ControllerPosition> targetController,
            std::chrono::system_clock::time_point expiresAt);
        [[nodiscard]] auto GetId() const -> int;
        [[nodiscard]] auto GetCallsign() const -> const std::string&;
        [[nodiscard]] auto GetDepartureAirfield() const -> const std::string&;
        [[nodiscard]] auto GetSid() const -> const std::string&;
        [[nodiscard]] auto GetDestinationAirfield() const -> const std::string&;
        [[nodiscard]] auto GetSendingControllerId() const -> int;
        [[nodiscard]] auto GetTargetControllerId() const -> int;
        [[nodiscard]] auto GetSendingController() const -> std::shared_ptr<Controller::ControllerPosition>;
        [[nodiscard]] auto GetTargetController() const -> std::shared_ptr<Controller::ControllerPosition>;
        [[nodiscard]] auto GetExpiresAt() const -> const std::chrono::system_clock::time_point&;
        [[nodiscard]] auto IsAcknowledged() const -> bool;
        [[nodiscard]] auto GetAcknowledgedAt() const -> const std::chrono::system_clock::time_point&;
        [[nodiscard]] auto GetCreatedAt() const -> const std::chrono::system_clock::time_point&;
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
        std::shared_ptr<Controller::ControllerPosition> sendingController;

        // Who the prenote was aimed at
        std::shared_ptr<Controller::ControllerPosition> targetController;

        // What time the prenote expires
        std::chrono::system_clock::time_point expiresAt;

        // What time the prenote expires
        std::chrono::system_clock::time_point acknowledgedAt;

        // When was it created
        std::chrono::system_clock::time_point createdAt;
    };
} // namespace UKControllerPlugin::Prenote
