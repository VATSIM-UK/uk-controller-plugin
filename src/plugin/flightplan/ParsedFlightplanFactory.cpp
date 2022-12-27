#include "FlightplanPoint.h"
#include "ParsedFlightplan.h"
#include "ParsedFlightplanFactory.h"
#include "euroscope/EuroscopeCoordinateWrapper.h"
#include "euroscope/EuroscopeExtractedRouteInterface.h"

namespace UKControllerPlugin::Flightplan {
    auto ParseFlightplanFromEuroscope(Euroscope::EuroscopeExtractedRouteInterface& euroscopePlan)
        -> std::shared_ptr<ParsedFlightplan>
    {
        auto flightplan = std::make_shared<ParsedFlightplan>();

        for (int i = 0; i < euroscopePlan.GetPointsNumber(); i++) {
            flightplan->AddPoint(std::make_shared<FlightplanPoint>(
                i,
                euroscopePlan.GetPointName(i),
                std::make_shared<Euroscope::EuroscopeCoordinateWrapper>(euroscopePlan.GetPointPosition(i))));
        }

        return flightplan;
    }

} // namespace UKControllerPlugin::Flightplan
