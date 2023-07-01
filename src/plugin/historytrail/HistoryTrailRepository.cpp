#include "AircraftHistoryTrail.h"
#include "HistoryTrailRepository.h"

namespace UKControllerPlugin::HistoryTrail {

    /*
        Returns an aircraft in the history trail.
    */
    auto HistoryTrailRepository::GetAircraft(const std::string& callsign) -> std::shared_ptr<AircraftHistoryTrail>
    {
        if (!this->HasAircraft(callsign)) {
            return nullptr;
        }

        return this->trailMap[callsign];
    }

    /*
        Returns whether or not the repository knows about a particular callsign.
    */
    auto HistoryTrailRepository::HasAircraft(const std::string& callsign) const -> bool
    {
        return this->trailMap.contains(callsign);
    }

    /*
        Adds an aircraft to the history trail repository, if it doesn't already
        exist.
    */
    void HistoryTrailRepository::RegisterAircraft(std::shared_ptr<AircraftHistoryTrail> trail)
    {
        if (this->HasAircraft(trail->GetCallsign())) {
            return;
        }

        // Add to the map and vector.
        this->trailMap[trail->GetCallsign()] = trail;
        this->trailData.push_back(trail);
    }

    /*
        Removes an aircraft from the history trail repository, if known.
    */
    void HistoryTrailRepository::UnregisterAircraft(const std::string& callsign)
    {
        if (!this->HasAircraft(callsign)) {
            return;
        }

        // Remove from the map and the vector
        this->trailMap.erase(callsign);
        this->trailData.erase(
            std::remove_if(
                this->trailData.begin(),
                this->trailData.end(),
                [callsign](std::shared_ptr<AircraftHistoryTrail> trail) { return trail->GetCallsign() == callsign; }),
            this->trailData.end());
    }
} // namespace UKControllerPlugin::HistoryTrail
