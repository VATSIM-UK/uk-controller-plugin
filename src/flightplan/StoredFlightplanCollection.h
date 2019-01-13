#pragma once
#include "flightplan/StoredFlightplan.h"

namespace UKControllerPlugin {
namespace Flightplan {

/*
    A collection of (local) flightplan objects. Also provides functions
    as to when the flightplan is to be invalidated.
*/
class StoredFlightplanCollection
{
    public:
        // Public type definitions for a custom iterator over the class.
        typedef std::map<std::string, std::unique_ptr<UKControllerPlugin::Flightplan::StoredFlightplan>> FlightplanMap;
        typedef FlightplanMap::iterator iterator;
        typedef FlightplanMap::const_iterator const_iterator;
        iterator begin(void) { return flightplans.begin(); }
        iterator end(void) { return flightplans.end(); }
        const_iterator cbegin() const { return flightplans.cbegin(); }
        const_iterator cend() const { return flightplans.cend(); }

        UKControllerPlugin::Flightplan::StoredFlightplan & GetFlightplanForCallsign(std::string callsign) const;
        bool HasFlightplanForCallsign(std::string callsign) const;
        void RemoveTimedOutPlans(void);
        void RemovePlanByCallsign(std::string callsign);
        void UpdatePlan(StoredFlightplan flightplan);

    private:

        // Storage for the flightplans - plus iterators
        FlightplanMap flightplans;
};

}  // namespace Flightplan
}  // namespace UKControllerPlugin
