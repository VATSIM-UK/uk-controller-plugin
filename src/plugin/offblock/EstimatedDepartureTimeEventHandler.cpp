#include "EstimatedDepartureTimeEventHandler.h"
#include "datablock/DisplayTime.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "flightplan/StoredFlightplanCollection.h"
#include "helper/HelperFunctions.h"
#include "tag/TagData.h"

using UKControllerPlugin::HelperFunctions;
using UKControllerPlugin::Datablock::DisplayTime;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Flightplan::StoredFlightplanCollection;
using UKControllerPlugin::Tag::TagData;

namespace UKControllerPlugin::Datablock {

    EstimatedDepartureTimeEventHandler::EstimatedDepartureTimeEventHandler(
        const StoredFlightplanCollection& storedFlightplans, const DisplayTime& displayTime)
        : storedFlightplans(storedFlightplans), displayTime(displayTime)
    {
    }

    auto EstimatedDepartureTimeEventHandler::GetTagItemDescription(int tagItemId) const -> std::string
    {
        return "Estimated Departure Time";
    }

    void EstimatedDepartureTimeEventHandler::SetTagItemData(TagData& tagData)
    {
        if (!this->storedFlightplans.HasFlightplanForCallsign(tagData.GetFlightplan().GetCallsign())) {
            tagData.SetItemString(this->displayTime.GetUnknownTimeFormat());
            return;
        }

        std::chrono::system_clock::time_point edt =
            this->storedFlightplans.GetFlightplanForCallsign(tagData.GetFlightplan().GetCallsign())
                .GetEstimatedDepartureTime();

        // If no valid EOBT, nothing to do
        if (edt == (std::chrono::system_clock::time_point::max)()) {
            tagData.SetItemString(this->displayTime.GetUnknownTimeFormat());
            return;
        }

        tagData.SetItemString(this->displayTime.FromTimePoint(edt));
    }

    /*
        If a flightplan is updated, make sure the EDT is set
    */
    void EstimatedDepartureTimeEventHandler::FlightPlanEvent(
        EuroScopeCFlightPlanInterface& flightPlan, EuroScopeCRadarTargetInterface& radarTarget)
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
    void EstimatedDepartureTimeEventHandler::FlightPlanDisconnectEvent(EuroScopeCFlightPlanInterface& flightPlan)
    {
    }

    /*
        Nothing to do here
    */
    void EstimatedDepartureTimeEventHandler::ControllerFlightPlanDataEvent(
        EuroScopeCFlightPlanInterface& flightPlan, int dataType)
    {
    }
} // namespace UKControllerPlugin::Datablock
