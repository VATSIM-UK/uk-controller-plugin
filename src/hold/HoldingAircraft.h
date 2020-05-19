#pragma once

namespace UKControllerPlugin {
    namespace Hold {

        /*
            Data about a holding aircraft
        */
        class HoldingAircraft
        {
            public:
                HoldingAircraft(std::string callsign, std::string assignedHold);
                HoldingAircraft(std::string callsign, std::set<std::string> proximityHolds);

                void AddProximityHold(std::string hold);
                std::string GetAssignedHold(void) const;
                const std::chrono::system_clock::time_point& GetAssignedHoldEntryTime(void) const;
                std::string GetCallsign(void) const;
                const std::set<std::string> GetProximityHolds(void) const;
                bool IsInAnyHold(void) const;
                bool IsInHold(std::string hold) const;
                bool IsInHoldProximity(std::string hold) const;
                void SetAssignedHold(std::string hold);
                void RemoveAssignedHold(void);
                void RemoveProximityHold(std::string hold);

                const std::string noHoldAssigned = "";

            private:

                // The callsign of the aircraft
                const std::string callsign;

                // The time the aircraft entered the hold
                std::chrono::system_clock::time_point entryTime;

                // The assigned hold of the aircraft (if any)
                std::string assignedHold = noHoldAssigned;

                // The holds against which the aircraft is the the vicinity
                std::set<std::string> proximityHolds;
        };

    }  // namespace Hold
}  // namespace UKControllerPlugin
