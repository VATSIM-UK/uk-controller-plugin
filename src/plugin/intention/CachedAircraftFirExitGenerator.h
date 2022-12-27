#pragma once
#include "AircraftFirExitGenerator.h"

namespace UKControllerPlugin::IntentionCode {
    struct AircraftFirExit;

    /*
        Generates aircraft FIR exit data and caches the result for future use.
    */
    class CachedAircraftFirExitGenerator : public AircraftFirExitGenerator
    {
        public:
        void AddCacheEntry(const std::shared_ptr<AircraftFirExit>& entry);
        [[nodiscard]] auto GetCacheEntryForCallsign(const std::string& callsign) const
            -> std::shared_ptr<AircraftFirExit>;
        void RemoveCacheEntryForCallsign(const std::string& callsign);
        void ClearCache();
        /*
            Generates the aircrafts FIR exit point data, or nullptr if there are none.
        */
        [[nodiscard]] auto Generate(Euroscope::EuroScopeCFlightPlanInterface& flightplan)
            -> std::shared_ptr<AircraftFirExit> override;

        private:
        // The cache
        std::map<std::string, std::shared_ptr<AircraftFirExit>> cache;
    };
} // namespace UKControllerPlugin::IntentionCode
