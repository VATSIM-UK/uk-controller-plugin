#include "ActualOffBlockTimeEventHandler.h"
#include "datablock/DisplayTime.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "flightplan/StoredFlightplanCollection.h"
#include "tag/TagData.h"

using UKControllerPlugin::Datablock::DisplayTime;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Flightplan::StoredFlightplan;
using UKControllerPlugin::Flightplan::StoredFlightplanCollection;
using UKControllerPlugin::Tag::TagData;

namespace UKControllerPlugin::Datablock {
    ActualOffBlockTimeEventHandler::ActualOffBlockTimeEventHandler(
        const StoredFlightplanCollection& flightplans, const DisplayTime& displayTime)
        : flightplans(flightplans), displayTime(displayTime)
    {
    }

    /*
        Nothing to do here.
    */
    void ActualOffBlockTimeEventHandler::FlightPlanEvent(
        EuroScopeCFlightPlanInterface& flightPlan, EuroScopeCRadarTargetInterface& radarTarget)
    {
    }

    /*
        Nothing to do here.
    */
    void ActualOffBlockTimeEventHandler::FlightPlanDisconnectEvent(EuroScopeCFlightPlanInterface& flightPlan)
    {
    }

    /*
        If the controller owns the airport and the data type is push, set the AOBT.
    */
    void ActualOffBlockTimeEventHandler::ControllerFlightPlanDataEvent(
        EuroScopeCFlightPlanInterface& flightPlan, int dataType)
    {
        if (dataType != EuroScopePlugIn::CTR_DATA_TYPE_GROUND_STATE) {
            return;
        }

        if (!this->flightplans.HasFlightplanForCallsign(flightPlan.GetCallsign())) {
            return;
        }

        if (flightPlan.GetGroundState() != "PUSH") {
            return;
        }

        this->flightplans.GetFlightplanForCallsign(flightPlan.GetCallsign())
            .SetActualOffBlockTime(std::chrono::system_clock::now());
    }

    auto ActualOffBlockTimeEventHandler::GetTagItemDescription(int tagItemId) const -> std::string
    {
        return "Actual Off-block Time";
    }

    void ActualOffBlockTimeEventHandler::SetTagItemData(TagData& tagData)
    {
        const auto& flightplan = tagData.GetFlightplan();
        if (!this->flightplans.HasFlightplanForCallsign(flightplan.GetCallsign())) {
            tagData.SetItemString(this->displayTime.GetUnknownTimeFormat());
            return;
        }

        std::chrono::system_clock::time_point offBlock =
            this->flightplans.GetFlightplanForCallsign(flightplan.GetCallsign()).GetActualOffBlockTime();

        tagData.SetItemString(
            offBlock == (std::chrono::system_clock::time_point::max)() ? this->displayTime.GetUnknownTimeFormat()
                                                                       : this->displayTime.FromTimePoint(offBlock));
    }
} // namespace UKControllerPlugin::Datablock
