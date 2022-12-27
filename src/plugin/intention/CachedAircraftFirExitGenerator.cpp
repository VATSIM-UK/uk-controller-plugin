#include "AircraftFirExit.h"
#include "CachedAircraftFirExitGenerator.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "flightplan/FlightplanPoint.h"
#include "flightplan/ParsedFlightplan.h"

namespace UKControllerPlugin::IntentionCode
{
    void CachedAircraftFirExitGenerator::AddCacheEntry(std::shared_ptr<AircraftFirExit> entry)
    {
        cache[entry->callsign] = entry;
    }

    auto CachedAircraftFirExitGenerator::GetCacheEntryForCallsign(const std::string& callsign) const
        -> std::shared_ptr<AircraftFirExit>
    {
        return cache.contains(callsign)
            ? cache.at(callsign)
            : nullptr;
    }

    void CachedAircraftFirExitGenerator::RemoveCacheEntryForCallsign(const std::string& callsign)
    {
        cache.erase(callsign);
    }

    void CachedAircraftFirExitGenerator::ClearCache()
    {
        cache.clear();
    }

    auto CachedAircraftFirExitGenerator::Generate(Euroscope::EuroScopeCFlightPlanInterface& flightplan) -> std::shared_ptr<AircraftFirExit> override
    {
        auto exit = AircraftFirExit;
        exit.callsign = flightplan.GetCallsign();

        const auto parsedFlightplan = flightplan.GetParsedFlightplan();
        int i = 0;
        while (true) {
            const auto point = parsedFlightplan->PointAtIndex(i);
            if (!point) {
                break;
            }

            i++;
        }
        
        return std::make_shared<AircraftFirExit>(exit);
    }

} // namespace UKControllerPlugin::IntentionCode