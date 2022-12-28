#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "ExitCone.h"
#include "euroscope/EuroscopeCoordinateInterface.h"
#include "flightplan/FlightplanPoint.h"
#include "flightplan/ParsedFlightplan.h"

namespace UKControllerPlugin::IntentionCode {
    ExitCone::ExitCone(int startDirection, int endDirection)
        : startDirection(startDirection), endDirection(endDirection)
    {
    }

    auto ExitCone::AircraftIsExiting(
        const Flightplan::FlightplanPoint& flightplanPoint, Euroscope::EuroScopeCFlightPlanInterface& flightplan) const
        -> bool
    {
        const auto nextPoint = flightplan.GetParsedFlightplan()->PointByIndex(flightplanPoint.Index() + 1);

        if (!nextPoint) {
            return false;
        }

        const auto exitDirection = (int)flightplanPoint.Position().DirectionTo(nextPoint->Position());

        return endDirection <= startDirection ? exitDirection >= startDirection || exitDirection <= endDirection
                                              : exitDirection >= startDirection && exitDirection <= endDirection;
    }

    auto ExitCone::StartDirection() const -> int
    {
        return startDirection;
    }

    auto ExitCone::EndDirection() const -> int
    {
        return endDirection;
    }
} // namespace UKControllerPlugin::IntentionCode
