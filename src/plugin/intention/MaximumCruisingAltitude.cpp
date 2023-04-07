#include "MaximumCruisingAltitude.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"

namespace UKControllerPlugin::IntentionCode {

    MaximumCruisingAltitude::MaximumCruisingAltitude(int altitude) : altitude(altitude)
    {
    }

    auto MaximumCruisingAltitude::Passes(
        const Euroscope::EuroScopeCFlightPlanInterface& flightplan,
        const Euroscope::EuroScopeCRadarTargetInterface& radarTarget) -> bool
    {
        return flightplan.GetCruiseLevel() <= altitude;
    }

    auto MaximumCruisingAltitude::Altitude() const -> int
    {
        return altitude;
    }
} // namespace UKControllerPlugin::IntentionCode
