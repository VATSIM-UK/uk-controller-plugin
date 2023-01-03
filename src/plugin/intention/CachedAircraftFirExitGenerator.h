#pragma once
#include "AircraftFirExitGenerator.h"
#include "flightplan/FlightPlanEventHandlerInterface.h"

namespace UKControllerPlugin::IntentionCode {
    struct AircraftFirExit;
    class FirExitPointCollection;

    /*
        Generates aircraft FIR exit data and caches the result for future use.
    */
    class CachedAircraftFirExitGenerator : public AircraftFirExitGenerator,
                                           public Flightplan::FlightPlanEventHandlerInterface
    {
        public:
        CachedAircraftFirExitGenerator(std::shared_ptr<const FirExitPointCollection> firExitPoints);
        void AddCacheEntry(const std::shared_ptr<AircraftFirExit>& entry);
        void FlightPlanEvent(
            Euroscope::EuroScopeCFlightPlanInterface& flightPlan,
            Euroscope::EuroScopeCRadarTargetInterface& radarTarget) override;
        void FlightPlanDisconnectEvent(Euroscope::EuroScopeCFlightPlanInterface& flightPlan) override;
        void ControllerFlightPlanDataEvent(
            UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightPlan, int dataType) override;
        [[nodiscard]] auto GetCacheEntryForCallsign(const std::string& callsign) const
            -> std::shared_ptr<AircraftFirExit>;
        void RemoveCacheEntryForCallsign(const std::string& callsign);
        /*
            Generates the aircrafts FIR exit point data, or nullptr if there are none.
        */
        [[nodiscard]] auto Generate(const Euroscope::EuroScopeCFlightPlanInterface& flightplan)
            -> std::shared_ptr<AircraftFirExit> override;

        private:
        // The FIR exit points
        std::shared_ptr<const FirExitPointCollection> firExitPoints;

        // The cache
        std::map<std::string, std::shared_ptr<AircraftFirExit>> cache;
    };
} // namespace UKControllerPlugin::IntentionCode
