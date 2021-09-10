#include "HoldingAircraft.h"

namespace UKControllerPlugin::Hold {

    HoldingAircraft::HoldingAircraft(std::string callsign, std::string assignedHold)
        : callsign(std::move(callsign)), entryTime(std::chrono::system_clock::now()),
          assignedHold(std::move(assignedHold))
    {
    }

    HoldingAircraft::HoldingAircraft(std::string callsign, std::set<std::string> proximityHolds)
        : callsign(std::move(callsign)), proximityHolds(std::move(proximityHolds))
    {
    }

    void HoldingAircraft::AddProximityHold(const std::string& hold)
    {
        this->proximityHolds.insert(hold);
    }

    auto HoldingAircraft::GetAssignedHold() const -> std::string
    {
        return this->assignedHold;
    }

    auto HoldingAircraft::GetAssignedHoldEntryTime() const -> const std::chrono::system_clock::time_point&
    {
        return this->entryTime;
    }

    auto HoldingAircraft::GetCallsign() const -> std::string
    {
        return this->callsign;
    }

    auto HoldingAircraft::GetProximityHolds() const -> std::set<std::string>
    {
        return this->proximityHolds;
    }

    auto HoldingAircraft::IsInAnyHold() const -> bool
    {
        return !this->proximityHolds.empty() || this->assignedHold != this->noHoldAssigned;
    }

    auto HoldingAircraft::IsInHold(const std::string& hold) const -> bool
    {
        return this->IsInHoldProximity(hold) || this->assignedHold == hold;
    }

    auto HoldingAircraft::IsInHoldProximity(const std::string& hold) const -> bool
    {
        return this->proximityHolds.find(hold) != this->proximityHolds.cend();
    }

    void HoldingAircraft::SetAssignedHold(std::string hold)
    {
        this->assignedHold = std::move(hold);
        this->entryTime = std::chrono::system_clock::now();
    }

    void HoldingAircraft::RemoveAssignedHold()
    {
        this->assignedHold = this->noHoldAssigned;
    }

    void HoldingAircraft::RemoveProximityHold(const std::string& hold)
    {
        auto proximity = this->proximityHolds.find(hold);
        if (proximity == this->proximityHolds.cend()) {
            return;
        }

        this->proximityHolds.erase(proximity);
    }

    auto HoldingAircraft::GetNoHoldAssigned() const -> const std::string&
    {
        return this->noHoldAssigned;
    }
} // namespace UKControllerPlugin::Hold
