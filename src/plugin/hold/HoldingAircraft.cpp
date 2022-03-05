#include "HoldingAircraft.h"

namespace UKControllerPlugin::Hold {

    HoldingAircraft::HoldingAircraft(std::string callsign, std::string assignedHold)
        : callsign(std::move(callsign)), assignedHold(std::move(assignedHold))
    {
    }

    HoldingAircraft::HoldingAircraft(std::string callsign, std::shared_ptr<ProximityHold> proximityHold)
        : callsign(std::move(callsign)), proximityHolds({proximityHold})
    {
    }

    void HoldingAircraft::AddProximityHold(std::shared_ptr<ProximityHold> hold)
    {
        this->proximityHolds.insert(hold);
    }

    auto HoldingAircraft::GetAssignedHold() const -> std::string
    {
        return this->assignedHold;
    }

    auto HoldingAircraft::GetCallsign() const -> std::string
    {
        return this->callsign;
    }

    auto HoldingAircraft::GetProximityHolds() const
        -> const std::set<std::shared_ptr<ProximityHold>, CompareProximityHolds>&
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

    auto HoldingAircraft::GetProximityHold(const std::string& hold) const -> std::shared_ptr<ProximityHold>
    {
        auto proximity = this->proximityHolds.find(hold);
        return proximity == this->proximityHolds.cend() ? nullptr : *proximity;
    }
} // namespace UKControllerPlugin::Hold
