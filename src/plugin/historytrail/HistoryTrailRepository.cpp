#include "pch/pch.h"
#include "historytrail/HistoryTrailRepository.h"
#include "historytrail/AircraftHistoryTrail.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"

namespace UKControllerPlugin {
    namespace HistoryTrail {

        HistoryTrailRepository::HistoryTrailRepository(void)
        {

        }

        HistoryTrailRepository::~HistoryTrailRepository(void)
        {

        }

        /*
            Returns an aircraft in the history trail.
        */
        std::shared_ptr<AircraftHistoryTrail> HistoryTrailRepository::GetAircraft(std::string callsign)
        {
            if (!this->HasAircraft(callsign)) {
                return NULL;
            }

            return this->trailData[callsign];
        }

        /*
            Returns whether or not the repository knows about a particular callsign.
        */
        bool HistoryTrailRepository::HasAircraft(std::string callsign) const
        {
            return this->trailData.count(callsign) == 1;
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

            this->trailData[trail->GetCallsign()] = trail;
        }

        /*
            Removes an aircraft from the history trail repository, if known.
        */
        void HistoryTrailRepository::UnregisterAircraft(std::string callsign)
        {
            if (!this->HasAircraft(callsign)) {
                return;
            }

            // Remove from the map and destroy the object.
            this->trailData.erase(callsign);
        }
    }  // namespace HistoryTrail
}  // namespace UKControllerPlugin
