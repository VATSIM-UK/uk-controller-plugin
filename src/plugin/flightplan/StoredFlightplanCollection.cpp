#include "flightplan/StoredFlightplanCollection.h"
#include "flightplan/StoredFlightplan.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"

using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Flightplan::StoredFlightplan;

namespace UKControllerPlugin {
    namespace Flightplan {

        StoredFlightplan& StoredFlightplanCollection::GetFlightplanForCallsign(std::string callsign) const
        {
            if (!this->HasFlightplanForCallsign(callsign)) {
                LogError("Attempted to reference flightplan for " + callsign + ", which is not stored");
                throw std::out_of_range("Flightplan for " + callsign + " not found.");
            }

            return *this->flightplans.find(callsign)->second;
        }

        /*
            Returns true if we have a flightplan for a given callsign.
        */
        bool StoredFlightplanCollection::HasFlightplanForCallsign(std::string callsign) const
        {
            return this->flightplans.find(callsign) != this->flightplans.cend();
        }

        /*
            Removes a plan for a given callsign, if it exists.
        */
        void StoredFlightplanCollection::RemovePlanByCallsign(std::string callsign)
        {
            std::map<std::string, std::unique_ptr<StoredFlightplan>>::iterator plan = this->flightplans.find(callsign);

            if (plan != this->flightplans.end()) {
                this->flightplans.erase(plan);
            }
        }

        /*
            Removes all plans that are deemed to have timed out.
        */
        void StoredFlightplanCollection::RemoveTimedOutPlans()
        {
            for (std::map<std::string, std::unique_ptr<StoredFlightplan>>::iterator it = this->flightplans.begin();
                 it != this->flightplans.end();) {
                if (it->second->HasTimedOut()) {
                    LogDebug("Stored flightplan for " + it->second->GetCallsign() + " has timed out");
                    this->flightplans.erase(it++);
                } else {
                    ++it;
                }
            }
        }

        /*
            Updates a plan in the collection, or adds it if doesn't exist.
        */
        void StoredFlightplanCollection::UpdatePlan(StoredFlightplan flightplan)
        {
            if (!this->HasFlightplanForCallsign(flightplan.GetCallsign())) {
                LogDebug("Now tracking flightplan data for " + flightplan.GetCallsign());
                this->flightplans[flightplan.GetCallsign()] = std::make_unique<StoredFlightplan>(flightplan);
                return;
            }

            *this->flightplans[flightplan.GetCallsign()] = flightplan;
        }
    } // namespace Flightplan
} // namespace UKControllerPlugin
