#include "RoutingVia.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "flightplan/ParsedFlightplan.h"

namespace UKControllerPlugin::IntentionCode {

    RoutingVia::RoutingVia(std::string via) : via(std::move(via))
    {
    }

    auto RoutingVia::Passes(
        Euroscope::EuroScopeCFlightPlanInterface& flightplan, Euroscope::EuroScopeCRadarTargetInterface& radarTarget)
        -> bool
    {
        return flightplan.GetParsedFlightplan()->HasPointByIdentifier(via);
    }

    auto RoutingVia::Via() const -> const std::string&
    {
        return via;
    }
} // namespace UKControllerPlugin::IntentionCode
