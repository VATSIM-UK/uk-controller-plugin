#include "FlightplanPoint.h"
#include "ParsedFlightplan.h"
#include "ParsedFlightplanFactory.h"
#include "euroscope/EuroscopeExtractedRouteInterface.h"

namespace UKControllerPlugin::Flightplan {
    auto ParseFlightplanFromEuroscope(Euroscope::EuroscopeExtractedRouteInterface& euroscopePlan)
        -> std::shared_ptr<ParsedFlightplan>
    {
        auto flightplan = std::make_shared<ParsedFlightplan>();
        
        for (i = 0; i < euroscopePlan.GetPointsNumber(); i++) {
            flightplan->AddPoint(std::make_shared<FlightplanPoint>(
                i,
                euroscopePlan.GetPointName(i),
                euroscopePlan.GetPointPosition(i)
            ));
        }

        return flightplan;
    }

} // namespace UKControllerPlugin::Flightplan