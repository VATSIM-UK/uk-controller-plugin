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

    auto ArrivalAirfields::Airfields() const -> const std::set<std::string>&
    {
        return airfields;
    }
} // namespace UKControllerPlugin::IntentionCode
