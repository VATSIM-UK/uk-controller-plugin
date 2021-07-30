#include "pch/pch.h"
#include "offblock/EstimatedOffBlockTimeEventHandler.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "flightplan/StoredFlightplanCollection.h"
#include "flightplan/StoredFlightplan.h"

using UKControllerPlugin::Flightplan::StoredFlightplanCollection;
using UKControllerPlugin::Datablock::DisplayTime;
using UKControllerPlugin::Tag::TagData;

namespace UKControllerPlugin {
namespace Datablock {

EstimatedOffBlockTimeEventHandler::EstimatedOffBlockTimeEventHandler(
    const StoredFlightplanCollection & storedFlightplans,
    const DisplayTime & displayTime
) : storedFlightplans(storedFlightplans), displayTime(displayTime)
{

}

std::string EstimatedOffBlockTimeEventHandler::GetTagItemDescription(int tagItemId) const
{
    return "Estimated Off-block Time";
}

void EstimatedOffBlockTimeEventHandler::SetTagItemData(TagData& tagData)
{
    if (!this->storedFlightplans.HasFlightplanForCallsign(tagData.flightPlan.GetCallsign())) {
        tagData.SetItemString(this->displayTime.GetUnknownTimeFormat());
        return;
    }

    std::chrono::system_clock::time_point eobt = this->storedFlightplans
        .GetFlightplanForCallsign(tagData.flightPlan.GetCallsign())
        .GetExpectedOffBlockTime();

    // If no valid EOBT, nothing to do
    if (eobt == (std::chrono::system_clock::time_point::max)()) {
        tagData.SetItemString(this->displayTime.GetUnknownTimeFormat());
        return;
    }

    tagData.SetItemString(this->displayTime.FromTimePoint(eobt));
}

}  // namespace Datablock
}  // namespace UKControllerPlugin
