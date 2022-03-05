#pragma once

namespace UKControllerPlugin::Hold {
    /**
     * Represents an aircraft in proximity to a hold
     */
    class ProximityHold
    {
        public:
        ProximityHold(std::string callsign, std::string navaid);
        ProximityHold(std::string callsign, std::string navaid, std::chrono::system_clock::time_point enteredAt);
        [[nodiscard]] auto Callsign() const -> const std::string&;
        [[nodiscard]] auto Navaid() const -> const std::string&;
        void Enter();
        [[nodiscard]] auto EnteredAt() const -> const std::chrono::system_clock::time_point&;
        [[nodiscard]] auto HasEntered() const -> bool;

        private:
        // The entering time, if the aircraft has never entered
        inline static const std::chrono::system_clock::time_point NOT_ENTERED =
            (std::chrono::system_clock::time_point::min)();

        // The callsign
        std::string callsign;

        // The navaid for the hold
        std::string navaid;

        // The time the aircraft entered the hold, if it has.
        std::chrono::system_clock::time_point enteredAt = NOT_ENTERED;
    };
} // namespace UKControllerPlugin::Hold
