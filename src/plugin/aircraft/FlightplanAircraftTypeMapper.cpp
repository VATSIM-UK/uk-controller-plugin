#include "AircraftTypeCollection.h"
#include "FlightplanAircraftTypeMapper.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"

namespace UKControllerPlugin::Aircraft {

    FlightplanAircraftTypeMapper::FlightplanAircraftTypeMapper(const AircraftTypeCollection& collection)
        : collection(collection)
    {
    }

    auto
    FlightplanAircraftTypeMapper::MapForFlightplan(const Euroscope::EuroScopeCFlightPlanInterface& flightplan) const
        -> std::shared_ptr<AircraftType>
    {
        return collection.GetByIcaoCode(flightplan.GetAircraftType());
    }
} // namespace UKControllerPlugin::Aircraft
