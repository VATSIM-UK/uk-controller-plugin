#include "pch/stdafx.h"
#include "handoff/HandoffEventHandler.h"

using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;

namespace UKControllerPlugin {
    namespace Handoff {

        HandoffEventHandler::HandoffEventHandler(
            const HandoffCollection handoffs,
            const ActiveCallsignCollection& callsigns
        ) : handoffs(std::move(handoffs)), callsigns(std::move(callsigns))
        {

        }

        std::string HandoffEventHandler::GetTagItemDescription(void) const
        {
            return "Departure Handoff Next Controller";
        }

        std::string HandoffEventHandler::GetTagItemData(
            EuroScopeCFlightPlanInterface& flightPlan,
            UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& radarTarget
        ) {
            return "-";
        }

        void HandoffEventHandler::FlightPlanEvent(
            EuroScopeCFlightPlanInterface& flightPlan,
            EuroScopeCRadarTargetInterface& radarTarget
        ) {
        }

        void HandoffEventHandler::FlightPlanDisconnectEvent(EuroScopeCFlightPlanInterface& flightPlan)
        {
        }

        void HandoffEventHandler::ControllerFlightPlanDataEvent(EuroScopeCFlightPlanInterface& flightPlan, int dataType)
        {
        }
    }  // namespace Handoff
}  // namespace UKControllerPlugin
