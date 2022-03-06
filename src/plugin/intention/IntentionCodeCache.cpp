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
