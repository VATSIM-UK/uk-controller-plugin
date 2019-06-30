#include "pch/stdafx.h"
#include "offblock/EstimatedDepartureTimeEventHandler.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "flightplan/StoredFlightplanCollection.h"
#include "flightplan/StoredFlightplan.h"
#include "flightplan/FlightPlanEventHandlerInterface.h"
#include "helper/HelperFunctions.h"

using UKControllerPlugin::Flightplan::StoredFlightplanCollection;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::HelperFunctions;
using UKControllerPlugin::Datablock::DisplayTime;

namespace UKControllerPlugin {
namespace Datablock {

EstimatedDepartureTimeEventHandler::EstimatedDepartureTimeEventHandler(
    const StoredFlightplanCollection & storedFlightplans,
    const DisplayTime & displayTime
) : storedFlightplans(storedFlightplans), displayTime(displayTime)
{

}

std::string EstimatedDepartureTimeEventHandler::GetTagItemDescription(void) const
{
    return "Estimated Departure Time";
}

std::string EstimatedDepartureTimeEventHandler::GetTagItemData(
    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightPlan,
    UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface & radarTarget
) {
    if (!this->storedFlightplans.HasFlightplanForCallsign(flightPlan.GetCallsign())) {
        return this->displayTime.GetUnknownTimeFormat();
    }

    std::chrono::system_clock::time_point edt = this->storedFlightplans
        .GetFlightplanForCallsign(flightPlan.GetCallsign())
        .GetEstimatedDepartureTime();

    // If no valid EOBT, nothing to do
    if (edt == (std::chrono::system_clock::time_point::max)()) {
        return this->displayTime.GetUnknownTimeFormat();
    }

    return this->displayTime.FromTimePoint(edt);
}

/*
    If a flightplan is updated, make sure the EDT is set
*/
void EstimatedDepartureTimeEventHandler::FlightPlanEvent(
    EuroScopeCFlightPlanInterface & flightPlan,
    EuroScopeCRadarTargetInterface & radarTarget
)
{
    if (!this->storedFlightplans.HasFlightplanForCallsign(flightPlan.GetCallsign())) {
        return;
    }

    this->storedFlightplans.GetFlightplanForCallsign(flightPlan.GetCallsign())
        .SetEstimatedDepartureTime(HelperFunctions::GetTimeFromNumberString(flightPlan.GetExpectedDepartureTime()));
}

/*
    Nothing to do here
*/
void EstimatedDepartureTimeEventHandler::FlightPlanDisconnectEvent(EuroScopeCFlightPlanInterface & flightPlan)
{

}

/*
    Nothing to do here
*/
void EstimatedDepartureTimeEventHandler::ControllerFlightPlanDataEvent(
    EuroScopeCFlightPlanInterface & flightPlan,
    int dataType
)
{

}
}  // namespace Datablock
}  // namespace UKControllerPlugin
