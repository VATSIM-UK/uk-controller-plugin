#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroscopeCoordinateInterface.h"
#include "flightplan/FlightplanPoint.h"
#include "flightplan/ParsedFlightplan.h"
#include "ExitCone.h"

namespace UKControllerPlugin::IntentionCode {
    ExitCone::ExitCone(int startDirection, int endDirection)
        : startDirection(startDirection), endDirection(endDirection)
    {
    }

    auto ExitCone::AircraftIsExiting(
        const Flightplan::FlightplanPoint& flightplanPoint, Euroscope::EuroScopeCFlightPlanInterface& flightplan)
        -> bool
    {
        const auto nextPoint = flightplan.GetParsedFlightplan()->PointByIndex(flightplanPoint.Index());

        if (!nextPoint) {
            return false;
        }

        const auto exitDirection = (int)flightplanPoint.Position().DirectionTo(nextPoint->Position());

        return endDirection <= startDirection ? exitDirection >= startDirection || exitDirection <= endDirection
                                              : exitDirection >= startDirection && exitDirection <= endDirection;
    }
} // namespace UKControllerPlugin::IntentionCode
