#pragma once
#include "wake/WakeCategoryMapperInterface.h"

namespace UKControllerPlugin::Aircraft {
    class AircraftTypeMapperInterface;
} // namespace UKControllerPlugin::Aircraft

namespace UKControllerPlugin::Wake {
    class WakeScheme;

    class FlightplanWakeCategoryMapper : public WakeCategoryMapperInterface
    {
        public:
        FlightplanWakeCategoryMapper(const WakeScheme& scheme, const Aircraft::AircraftTypeMapperInterface& aircraftTypes);
        auto MapForFlightplan(const Euroscope::EuroScopeCFlightPlanInterface& flightplan)
            -> std::shared_ptr<WakeCategory> override;

        private:
        // The wake scheme we're mapping for
        const WakeScheme& scheme;

        // The aircraft types
        const Aircraft::AircraftTypeMapperInterface& aircraftTypes;

        // Map of aircraft type => scheme so we don't have to look up each time
        std::map<std::string, std::shared_ptr<WakeCategory>> cache;
    };
} // namespace UKControllerPlugin::Wake
