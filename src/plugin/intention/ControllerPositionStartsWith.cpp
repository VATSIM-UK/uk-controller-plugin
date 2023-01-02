#include "ControllerPositionStartsWith.h"
#include "controller/ActiveCallsign.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPosition.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"

namespace UKControllerPlugin::IntentionCode {

    ControllerPositionStartsWith::ControllerPositionStartsWith(
        std::shared_ptr<const Controller::ActiveCallsignCollection> activeControllers, std::string pattern)
        : activeControllers(activeControllers), pattern(std::move(pattern))
    {
        assert(activeControllers && "active callsigns not set in ControllerPositionStartsWith");
    }

    auto ControllerPositionStartsWith::Passes(
        const Euroscope::EuroScopeCFlightPlanInterface& flightplan,
        const Euroscope::EuroScopeCRadarTargetInterface& radarTarget) -> bool
    {
        return activeControllers->UserHasCallsign() &&
               activeControllers->GetUserCallsign().GetNormalisedPosition().GetCallsign().substr(0, pattern.size()) ==
                   pattern;
    }

    auto ControllerPositionStartsWith::Pattern() const -> const std::string&
    {
        return pattern;
    }
} // namespace UKControllerPlugin::IntentionCode
