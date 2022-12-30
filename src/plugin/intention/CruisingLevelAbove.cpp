#include "CruisingLevelAbove.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"

namespace UKControllerPlugin::IntentionCode {

    CruisingLevelAbove::CruisingLevelAbove(int altitude) : altitude(altitude)
    {
    }

    auto CruisingLevelAbove::Passes(
        Euroscope::EuroScopeCFlightPlanInterface& flightplan, Euroscope::EuroScopeCRadarTargetInterface& radarTarget)
        -> bool
    {
        return flightplan.GetCruiseLevel() > altitude;
    }
} // namespace UKControllerPlugin::IntentionCode
