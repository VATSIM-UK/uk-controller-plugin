#include "EstimatedOffBlockTimeEventHandler.h"
#include "datablock/DisplayTime.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "flightplan/StoredFlightplanCollection.h"
#include "tag/TagData.h"

using UKControllerPlugin::Datablock::DisplayTime;
using UKControllerPlugin::Flightplan::StoredFlightplanCollection;
using UKControllerPlugin::Tag::TagData;

namespace UKControllerPlugin::Datablock {

    EstimatedOffBlockTimeEventHandler::EstimatedOffBlockTimeEventHandler(
        const StoredFlightplanCollection& storedFlightplans, const DisplayTime& displayTime)
        : storedFlightplans(storedFlightplans), displayTime(displayTime)
    {
    }

    auto EstimatedOffBlockTimeEventHandler::GetTagItemDescription(int tagItemId) const -> std::string
    {
        return "Estimated Off-block Time";
    }

    void EstimatedOffBlockTimeEventHandler::SetTagItemData(TagData& tagData)
    {
        if (!this->storedFlightplans.HasFlightplanForCallsign(tagData.GetFlightplan().GetCallsign())) {
            tagData.SetItemString(this->displayTime.GetUnknownTimeFormat());
            return;
        }

        std::chrono::system_clock::time_point eobt =
            this->storedFlightplans.GetFlightplanForCallsign(tagData.GetFlightplan().GetCallsign())
                .GetExpectedOffBlockTime();

        // If no valid EOBT, nothing to do
        if (eobt == (std::chrono::system_clock::time_point::max)()) {
            tagData.SetItemString(this->displayTime.GetUnknownTimeFormat());
            return;
        }

        tagData.SetItemString(this->displayTime.FromTimePoint(eobt));
    }

} // namespace UKControllerPlugin::Datablock
