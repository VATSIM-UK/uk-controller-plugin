#include "pch/stdafx.h"
#include "intention/IntentionCodeCache.h"
#include "euroscope/EuroscopeExtractedRouteInterface.h"

using UKControllerPlugin::IntentionCode::IntentionCodeData;
using UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface;

namespace UKControllerPlugin {
    namespace IntentionCode {

        /*
            Gets an intention code for a given aircraft.
        */
        std::string IntentionCodeCache::GetIntentionCodeForAircraft(std::string callsign) const
        {
            if (this->intentionCodeMap.count(callsign) == 0) {
                return "--";
            }

            return this->intentionCodeMap.at(callsign).intentionCode;
        }

        /*
            Returns true or false depending on whether we have an intention code cached.
        */
        bool IntentionCodeCache::HasIntentionCodeForAircraft(std::string callsign) const
        {
            return this->intentionCodeMap.count(callsign) > 0;
        }

        /*
            Returns true if the intention code is still valid.
        */
        bool IntentionCodeCache::IntentionCodeValid(std::string callsign, EuroscopeExtractedRouteInterface & route)
        {
            if (!this->HasIntentionCodeForAircraft(callsign)) {
                return false;
            }

            // If they don't have an exit point, or they're cleared direct beyond it but aren't yet close.
            if (this->intentionCodeMap[callsign].exitPointValid == false ||
                (route.GetPointsAssignedIndex() > this->intentionCodeMap[callsign].exitPointIndex &&
                    route.GetPointsCalculatedIndex() <= this->intentionCodeMap[callsign].exitPointIndex)
                ) {
                return true;
            }

            // If they've passed their exit point, then the intention code is no longer valid.
            if (route.GetPointDistanceInMinutes(
                this->intentionCodeMap[callsign].exitPointIndex) == this->exitPointPassed
            ) {
                return false;
            }

            return true;
        }

        /*
            Registers an aircraft with the cache.
        */
        void IntentionCodeCache::RegisterAircraft(std::string callsign, IntentionCodeData intentionCode)
        {
            if (this->intentionCodeMap.count(callsign) != 0) {
                return;
            }

            this->intentionCodeMap[callsign] =  intentionCode;
        }

        /*
            Returns the total number of intention codes we have cached.
        */
        size_t IntentionCodeCache::TotalCached(void) const
        {
            return this->intentionCodeMap.size();
        }

        /*
            Unregisters an aircraft with the intention code cache.
        */
        void IntentionCodeCache::UnregisterAircraft(std::string callsign)
        {
            if (this->intentionCodeMap.count(callsign) == 0) {
                return;
            }

            this->intentionCodeMap.erase(callsign);
        }
    }  // namespace IntentionCode
}  // namespace UKControllerPlugin
