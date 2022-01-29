#pragma once
#include "CompareProximityHolds.h"

namespace UKControllerPlugin::Hold {
    struct ProximityHold;

    /*
        Data about a holding aircraft
    */
    class HoldingAircraft
    {
        public:
        HoldingAircraft(std::string callsign, std::string assignedHold);
        HoldingAircraft(std::string callsign, std::shared_ptr<ProximityHold> proximityHold);
        void AddProximityHold(std::shared_ptr<ProximityHold> hold);
        [[nodiscard]] auto GetAssignedHold() const -> std::string;
        [[nodiscard]] auto GetCallsign() const -> std::string;
        [[nodiscard]] auto GetProximityHolds() const
            -> const std::set<std::shared_ptr<ProximityHold>, CompareProximityHolds>&;
        [[nodiscard]] auto GetProximityHold(const std::string& hold) const -> std::shared_ptr<ProximityHold>;
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

        // The assigned hold of the aircraft (if any)
        std::string assignedHold = noHoldAssigned;

        // The holds against which the aircraft is the the vicinity
        std::set<std::shared_ptr<ProximityHold>, CompareProximityHolds> proximityHolds;
    };
} // namespace UKControllerPlugin::Hold
