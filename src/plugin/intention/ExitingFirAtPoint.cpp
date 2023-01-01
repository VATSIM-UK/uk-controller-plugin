#include "AircraftFirExitGenerator.h"
#include "AircraftFirExit.h"
#include "ExitingFirAtPoint.h"
#include "FirExitPoint.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"

namespace UKControllerPlugin::IntentionCode {

    ExitingFirAtPoint::ExitingFirAtPoint(AircraftFirExitGenerator& firExitGenerator, int firExitPointId)
        : firExitGenerator(firExitGenerator), firExitPointId(firExitPointId)
    {
    }

    auto ExitingFirAtPoint::Passes(
        const Euroscope::EuroScopeCFlightPlanInterface& flightplan,
        const Euroscope::EuroScopeCRadarTargetInterface& radarTarget) -> bool
    {
        const auto firExit = firExitGenerator.Generate(flightplan);
        if (!firExit) {
            return false;
        }

        return (firExit->ukExitPoint && firExit->ukExitPoint->Id() == firExitPointId) ||
               (firExit->internalExitPoint && firExit->internalExitPoint->Id() == firExitPointId);
    }

    auto ExitingFirAtPoint::ExitPoint() const -> int
    {
        return firExitPointId;
    }
} // namespace UKControllerPlugin::IntentionCode
