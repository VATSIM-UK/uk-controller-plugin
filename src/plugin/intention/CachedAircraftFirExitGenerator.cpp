#include "AircraftFirExit.h"
#include "CachedAircraftFirExitGenerator.h"
#include "ExitDetermination.h"
#include "FirExitPoint.h"
#include "FirExitPointCollection.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "flightplan/FlightplanPoint.h"
#include "flightplan/ParsedFlightplan.h"

namespace UKControllerPlugin::IntentionCode {

    CachedAircraftFirExitGenerator::CachedAircraftFirExitGenerator(
        std::shared_ptr<FirExitPointCollection> firExitPoints)
        : firExitPoints(firExitPoints)
    {
        assert(firExitPoints && "FIR exit point collection not set");
    }

    void CachedAircraftFirExitGenerator::AddCacheEntry(const std::shared_ptr<AircraftFirExit>& entry)
    {
        cache[entry->callsign] = entry;
    }

    auto CachedAircraftFirExitGenerator::GetCacheEntryForCallsign(const std::string& callsign) const
        -> std::shared_ptr<AircraftFirExit>
    {
        return cache.contains(callsign) ? cache.at(callsign) : nullptr;
    }

    void CachedAircraftFirExitGenerator::RemoveCacheEntryForCallsign(const std::string& callsign)
    {
        cache.erase(callsign);
    }

    void CachedAircraftFirExitGenerator::ClearCache()
    {
        cache.clear();
    }

    auto CachedAircraftFirExitGenerator::Generate(Euroscope::EuroScopeCFlightPlanInterface& flightplan)
        -> std::shared_ptr<AircraftFirExit>
    {
        auto cachedEntry = GetCacheEntryForCallsign(flightplan.GetCallsign());
        if (cachedEntry) {
            return cachedEntry;
        }

        auto exit = AircraftFirExit{};
        exit.callsign = flightplan.GetCallsign();

        const auto parsedFlightplan = flightplan.GetParsedFlightplan();
        for (size_t i = 0; i < parsedFlightplan->CountPoints(); i++) {
            const auto flightplanPoint = parsedFlightplan->PointByIndex(i);

            // If we've reached the end of the flightplan, stop.
            if (!flightplanPoint) {
                break;
            }

            const auto firExitPoint = firExitPoints->PointByIdentifier(flightplanPoint->Identifier());
            if (!firExitPoint) {
                continue;
            }

            /*
             * If the aircraft is exiting between the two FIRs, we only care about recording this if we don't already
             * have an internal exit point and the aircraft is exiting.
             */
            if (firExitPoint->IsInternal()) {
                if (exit.internalExitPoint) {
                    continue;
                }

                if (firExitPoint->FirExitDetermination().AircraftIsExiting(*flightplanPoint, flightplan)) {
                    exit.internalExitPoint = firExitPoint;
                }

                continue;
            }

            /*
             * If the aircraft is not exiting between the two FIRs, then we have reached the UK exit and don't need
             * to check this any further.
             */
            if (firExitPoint->FirExitDetermination().AircraftIsExiting(*flightplanPoint, flightplan)) {
                exit.ukExitPoint = firExitPoint;
                break;
            }
        }

        const auto cacheItem = std::make_shared<AircraftFirExit>(exit);
        AddCacheEntry(cacheItem);

        return cacheItem;
    }

} // namespace UKControllerPlugin::IntentionCode
