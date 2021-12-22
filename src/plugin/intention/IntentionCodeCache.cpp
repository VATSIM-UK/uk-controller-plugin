#include "IntentionCodeCache.h"
#include "euroscope/EuroscopeExtractedRouteInterface.h"

using UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface;
using UKControllerPlugin::IntentionCode::IntentionCodeData;

namespace UKControllerPlugin::IntentionCode {

    /*
        Gets an intention code for a given aircraft.
    */
    std::string IntentionCodeCache::GetIntentionCodeForAircraft(const std::string& callsign) const
    {
        if (this->intentionCodeMap.count(callsign) == 0) {
            return "--";
        }

        return this->intentionCodeMap.at(callsign).intentionCode;
    }

    /*
        Returns true or false depending on whether we have an intention code cached.
    */
    bool IntentionCodeCache::HasIntentionCodeForAircraft(const std::string& callsign) const
    {
        return this->intentionCodeMap.count(callsign) > 0;
    }

    void IntentionCodeCache::Clear(void)
    {
        this->intentionCodeMap.clear();
    }

    /*
        Returns true if the intention code is still valid.
    */
    bool IntentionCodeCache::IntentionCodeValid(const std::string& callsign, EuroscopeExtractedRouteInterface& route)
    {
        if (!this->HasIntentionCodeForAircraft(callsign)) {
            return false;
        }

        // If they don't have an exit point, or they're cleared direct beyond it but aren't yet close.
        if (this->intentionCodeMap[callsign].exitPointIndex == IntentionCodeData::INVALID_EXIT_POINT ||
            (route.GetPointsAssignedIndex() > this->intentionCodeMap[callsign].exitPointIndex &&
             route.GetPointsCalculatedIndex() <= this->intentionCodeMap[callsign].exitPointIndex)) {
            return true;
        }

        // If they've passed their exit point, then the intention code is no longer valid.
        if (route.GetPointDistanceInMinutes(this->intentionCodeMap[callsign].exitPointIndex) == this->exitPointPassed) {
            return false;
        }

        return true;
    }

    /*
        Registers an aircraft with the cache.
    */
    void IntentionCodeCache::RegisterAircraft(const std::string& callsign, IntentionCodeData intentionCode)
    {
        if (this->intentionCodeMap.count(callsign) != 0) {
            return;
        }

        this->intentionCodeMap[callsign] = intentionCode;
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
    void IntentionCodeCache::UnregisterAircraft(const std::string& callsign)
    {
        if (this->intentionCodeMap.count(callsign) == 0) {
            return;
        }

        this->intentionCodeMap.erase(callsign);
    }

    auto IntentionCodeCache::GetDataForAircraft(const std::string& callsign) const -> IntentionCodeData
    {
        return this->intentionCodeMap.count(callsign) != 0 ? this->intentionCodeMap.at(callsign) : IntentionCodeData();
    }
} // namespace UKControllerPlugin::IntentionCode
