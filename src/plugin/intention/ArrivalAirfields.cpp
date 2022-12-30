#include "ArrivalAirfields.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"

namespace UKControllerPlugin::IntentionCode {

    ArrivalAirfields::ArrivalAirfields(std::set<std::string> airfields) : airfields(std::move(airfields))
    {
    }

    auto ArrivalAirfields::Passes(
        Euroscope::EuroScopeCFlightPlanInterface& flightplan, Euroscope::EuroScopeCRadarTargetInterface& radarTarget)
        -> bool
    {
        return airfields.contains(flightplan.GetDestination());
    }
} // namespace UKControllerPlugin::IntentionCode
