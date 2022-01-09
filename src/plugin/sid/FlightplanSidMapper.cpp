#include "FlightplanSidMapper.h"
#include "airfield/AirfieldCollection.h"
#include "airfield/AirfieldModel.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "runway/Runway.h"
#include "runway/RunwayCollection.h"
#include "sid/SidCollection.h"

namespace UKControllerPlugin::Sid {

    FlightplanSidMapper::FlightplanSidMapper(
        const Airfield::AirfieldCollection& airfields,
        const Runway::RunwayCollection& runways,
        const SidCollection& sids)
        : airfields(airfields), runways(runways), sids(sids)
    {
    }

    auto FlightplanSidMapper::MapFlightplanToSid(const Euroscope::EuroScopeCFlightPlanInterface& flightplan) const
        -> std::shared_ptr<StandardInstrumentDeparture>
    {
        const auto airfield = airfields.FetchAirfieldByIcao(flightplan.GetOrigin());
        if (!airfield) {
            return nullptr;
        }

        const auto runway = runways.GetByAirfieldAndIdentifier(airfield->Id(), flightplan.GetDepartureRunway());
        if (!runway) {
            return nullptr;
        }

        return sids.GetByRunwayIdAndIdentifier(runway->Id(), flightplan.GetSidName());
    }
} // namespace UKControllerPlugin::Sid
