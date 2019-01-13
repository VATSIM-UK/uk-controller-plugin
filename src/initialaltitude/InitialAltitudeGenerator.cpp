#include "pch/stdafx.h"
#include "initialaltitude/InitialAltitudeGenerator.h"

namespace UKControllerPlugin {
    namespace InitialAltitude {

        // Add a SID, throw an exception if already added
        void InitialAltitudeGenerator::AddSid(std::string airfield, std::string sidName, int altitude)
        {
            if (!this->altitudes.insert({ airfield + "." + sidName, altitude }).second) {
                LogWarning("Duplicate initial altitude for " + airfield + "." + sidName);
            }
        }

        /*
            Counts the number in the container.
        */
        int InitialAltitudeGenerator::Count(void) const
        {
            return this->altitudes.size();
        }

        /*
            Returns the initial altitude for a given SID from a given airport.
        */
        int InitialAltitudeGenerator::GetInitialAltitudeForDeparture(std::string origin, std::string sid) const
        {
            // SID unknown, return 0
            if (!this->HasSid(origin, sid)) {
                throw std::out_of_range("SID not found.");
            }

            return this->altitudes.find(this->GetSidIdentifier(origin, sid))->altitude;
        }

        /*
            Returns true if a SID is known to the class, false otherwise.
        */
        bool InitialAltitudeGenerator::HasSid(std::string origin, std::string sid) const
        {
            return this->altitudes.find(this->GetSidIdentifier(origin, sid)) != this->altitudes.cend();
        }

        /*
            Returns the standard identifier used for sids.
        */
        std::string InitialAltitudeGenerator::GetSidIdentifier(std::string origin, std::string sid) const
        {
            return origin + "." + sid;
        }
    }  // namespace InitialAltitude
}  // namespace UKControllerPlugin
