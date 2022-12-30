#include "ControllerPositionStartsWith.h"
#include "controller/ActiveCallsign.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPosition.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"

namespace UKControllerPlugin::IntentionCode {

    ControllerPositionStartsWith::ControllerPositionStartsWith(
        const UKControllerPlugin::Controller::ActiveCallsignCollection& activeControllers, std::string pattern)
        : activeControllers(activeControllers), pattern(std::move(pattern))
    {
    }

    auto ControllerPositionStartsWith::Passes(
        Euroscope::EuroScopeCFlightPlanInterface& flightplan, Euroscope::EuroScopeCRadarTargetInterface& radarTarget)
        -> bool
    {
        return activeControllers.UserHasCallsign() &&
               activeControllers.GetUserCallsign().GetNormalisedPosition().GetCallsign().substr(0, pattern.size()) ==
                   pattern;
    }
} // namespace UKControllerPlugin::IntentionCode
