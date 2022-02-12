#pragma once
#include "sid/SidMapperInterface.h"

namespace UKControllerPlugin {
    namespace Airfield {
        class AirfieldCollection;
    } // namespace Airfield
    namespace Euroscope {
        class EuroScopeCFlightPlanInterface;
    } // namespace Euroscope
    namespace Runway {
        class RunwayCollection;
    } // namespace Runway
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Sid {
    class SidCollection;
    class StandardInstrumentDeparture;

    /**
     * Given a flightplan, resolve it to its Standard Instrument Departure.
     */
    class FlightplanSidMapper : public Sid::SidMapperInterface
    {
        public:
        FlightplanSidMapper(
            const Airfield::AirfieldCollection& airfields,
            const Runway::RunwayCollection& runways,
            const SidCollection& sids);
        [[nodiscard]] auto MapFlightplanToSid(const Euroscope::EuroScopeCFlightPlanInterface& flightplan) const
            -> std::shared_ptr<StandardInstrumentDeparture>;

        private:
        // All the airfields
        const Airfield::AirfieldCollection& airfields;

        // All the runways
        const Runway::RunwayCollection& runways;

        // All the SIDs
        const SidCollection& sids;
    };
} // namespace UKControllerPlugin::Sid
