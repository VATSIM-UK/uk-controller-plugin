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

    auto CruisingLevelAbove::Level() const -> int
    {
        return altitude;
    }
} // namespace UKControllerPlugin::IntentionCode
