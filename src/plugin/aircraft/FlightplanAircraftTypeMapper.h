#pragma once
#include "aircraft/AircraftTypeMapperInterface.h"

namespace UKControllerPlugin::Aircraft {
    class AircraftType;
    class AircraftTypeCollection;

    class FlightplanAircraftTypeMapper : public AircraftTypeMapperInterface
    {
        public:
        FlightplanAircraftTypeMapper(const AircraftTypeCollection& collection);
        [[nodiscard]] auto MapForFlightplan(const Euroscope::EuroScopeCFlightPlanInterface& flightplan) const
            -> std::shared_ptr<AircraftType> override;

        private:
        // The aircraft types
        const AircraftTypeCollection& collection;
    };
} // namespace UKControllerPlugin::Aircraft
