#pragma once
#include "AircraftIntentionCodeGenerator.h"
#include "controller/ActiveCallsignEventHandlerInterface.h"
#include "flightplan/FlightPlanEventHandlerInterface.h"

namespace UKControllerPlugin::IntentionCode {

    class IntentionCodeCollection;
    class IntentionCodeEventHandlerCollection;

    /**
     * Generates intention codes for aircraft, and caches them.
     */
    class CachedAircraftIntentionCodeGenerator : public AircraftIntentionCodeGenerator,
                                                 public Controller::ActiveCallsignEventHandlerInterface,
                                                 public Flightplan::FlightPlanEventHandlerInterface
    {
        public:
        CachedAircraftIntentionCodeGenerator(
            const IntentionCodeCollection& intentionCodes, const IntentionCodeEventHandlerCollection& eventHandlers);
        void AddCacheEntry(const std::shared_ptr<AircraftIntentionCode>& entry);
        void FlightPlanEvent(
            Euroscope::EuroScopeCFlightPlanInterface& flightPlan,
            Euroscope::EuroScopeCRadarTargetInterface& radarTarget) override;
        void FlightPlanDisconnectEvent(Euroscope::EuroScopeCFlightPlanInterface& flightPlan) override;
        void ControllerFlightPlanDataEvent(
            UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightPlan, int dataType) override;
        [[nodiscard]] auto GetCacheEntryForCallsign(const std::string& callsign) const
            -> std::shared_ptr<AircraftIntentionCode>;
        void RemoveCacheEntryForCallsign(const std::string& callsign);
        /*
            Generates the aircrafts FIR exit point data, or nullptr if there are none.
        */
        [[nodiscard]] auto Generate(
            const Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            const Euroscope::EuroScopeCRadarTargetInterface& radarTarget) -> std::shared_ptr<AircraftIntentionCode>
                override;

        void ActiveCallsignAdded(const Controller::ActiveCallsign& callsign) override;
        void ActiveCallsignRemoved(const Controller::ActiveCallsign& callsign) override;

        private:
        // The intention codes
        const IntentionCodeCollection& intentionCodes;

        // Handles intention code events
        const IntentionCodeEventHandlerCollection& eventHandlers;

        // The cache
        std::map<std::string, std::shared_ptr<AircraftIntentionCode>> cache;
    };
} // namespace UKControllerPlugin::IntentionCode
