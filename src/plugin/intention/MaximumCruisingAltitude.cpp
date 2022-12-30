#include "MaximumCruisingAltitude.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"

namespace UKControllerPlugin::IntentionCode {

    MaximumCruisingAltitude::MaximumCruisingAltitude(int altitude) : altitude(altitude)
    {
    }

    auto MaximumCruisingAltitude::Passes(
        Euroscope::EuroScopeCFlightPlanInterface& flightplan, Euroscope::EuroScopeCRadarTargetInterface& radarTarget)
        -> bool
    {
        return flightplan.GetCruiseLevel() <= altitude;
    }
} // namespace UKControllerPlugin::IntentionCode
