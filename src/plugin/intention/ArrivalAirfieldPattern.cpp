#include "ArrivalAirfieldPattern.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"

namespace UKControllerPlugin::IntentionCode {

    ArrivalAirfieldPattern::ArrivalAirfieldPattern(std::string pattern) : pattern(std::move(pattern))
    {
    }

    auto ArrivalAirfieldPattern::Passes(
        const Euroscope::EuroScopeCFlightPlanInterface& flightplan,
        const Euroscope::EuroScopeCRadarTargetInterface& radarTarget) -> bool
    {
        return flightplan.GetDestination().empty() ? false
                                                   : flightplan.GetDestination().substr(0, pattern.size()) == pattern;
    }

    auto ArrivalAirfieldPattern::Pattern() const -> const std::string&
    {
        return pattern;
    }
} // namespace UKControllerPlugin::IntentionCode
