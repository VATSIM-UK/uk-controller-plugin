#include "FlightplanAirfieldHandoffMapper.h"
#include "HandoffCollection.h"
#include "airfield/AirfieldCollection.h"
#include "airfield/AirfieldModel.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"

namespace UKControllerPlugin::Handoff {

    FlightplanAirfieldHandoffMapper::FlightplanAirfieldHandoffMapper(
        const HandoffCollection& handoffs, const Airfield::AirfieldCollection& airfields)
        : handoffs(handoffs), airfields(airfields)
    {
    }

    auto
    FlightplanAirfieldHandoffMapper::MapForFlightplan(const Euroscope::EuroScopeCFlightPlanInterface& flightplan) const
        -> std::shared_ptr<HandoffOrder>
    {
        const auto airfield = airfields.FetchAirfieldByIcao(flightplan.GetOrigin());
        if (!airfield || !airfield->HasDefaultHandoff()) {
            return nullptr;
        }

        return handoffs.Get(airfield->DefaultHandoff());
    }
} // namespace UKControllerPlugin::Handoff
