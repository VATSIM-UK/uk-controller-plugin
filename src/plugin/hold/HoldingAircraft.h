#pragma once

namespace UKControllerPlugin::Hold {

    /*
        Data about a holding aircraft
    */
    class HoldingAircraft
    {
        public:
        HoldingAircraft(std::string callsign, std::string assignedHold);
        HoldingAircraft(std::string callsign, std::set<std::string> proximityHolds);

        void AddProximityHold(const std::string& hold);
        [[nodiscard]] auto GetAssignedHold() const -> std::string;
        [[nodiscard]] auto GetAssignedHoldEntryTime() const -> const std::chrono::system_clock::time_point&;
        [[nodiscard]] auto GetCallsign() const -> std::string;
        [[nodiscard]] auto GetProximityHolds() const -> std::set<std::string>;
        [[nodiscard]] auto IsInAnyHold() const -> bool;
        [[nodiscard]] auto IsInHold(const std::string& hold) const -> bool;
        [[nodiscard]] auto IsInHoldProximity(const std::string& hold) const -> bool;
        void SetAssignedHold(std::string hold);
        void RemoveAssignedHold();
        void RemoveProximityHold(const std::string& hold);
        [[nodiscard]] auto GetNoHoldAssigned() const -> const std::string&;

        private:
        // If there's no hold assigned
        const std::string noHoldAssigned;

        // The callsign of the aircraft
        const std::string callsign;

        // The time the aircraft entered the hold
        std::chrono::system_clock::time_point entryTime;

        // The assigned hold of the aircraft (if any)
        std::string assignedHold = noHoldAssigned;

        // The holds against which the aircraft is the the vicinity
        std::set<std::string> proximityHolds;
    };
} // namespace UKControllerPlugin::Hold
