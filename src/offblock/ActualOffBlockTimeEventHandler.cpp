#include "pch/stdafx.h"
#include "offblock/ActualOffBlockTimeEventHandler.h"
#include "flightplan/StoredFlightplanCollection.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"

using UKControllerPlugin::Flightplan::StoredFlightplanCollection;
using UKControllerPlugin::Flightplan::StoredFlightplan;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Datablock::DisplayTime;

namespace UKControllerPlugin {
    namespace Datablock {
        ActualOffBlockTimeEventHandler::ActualOffBlockTimeEventHandler(
            const StoredFlightplanCollection & flightplans,
            const DisplayTime displayTime
        )
            : flightplans(flightplans), displayTime(displayTime)
        {

        }

        /*
            Nothing to do here.
        */
        void ActualOffBlockTimeEventHandler::FlightPlanEvent(
            EuroScopeCFlightPlanInterface & flightPlan,
            EuroScopeCRadarTargetInterface & radarTarget
        ) {
        }

        /*
            Nothing to do here.
        */
        void ActualOffBlockTimeEventHandler::FlightPlanDisconnectEvent(EuroScopeCFlightPlanInterface & flightPlan)
        {
        }

        /*
            If the controller owns the airport and the data type is push, set the AOBT.
        */
        void ActualOffBlockTimeEventHandler::ControllerFlightPlanDataEvent(
            EuroScopeCFlightPlanInterface & flightPlan,
            int dataType
        ) {
            if (dataType != EuroScopePlugIn::CTR_DATA_TYPE_GROUND_STATE) {
                return;
            }

            if (!this->flightplans.HasFlightplanForCallsign(flightPlan.GetCallsign())) {
                return;
            }

            if (flightPlan.GetGroundState() != "PUSH") {
                return;
            }

            this->flightplans.GetFlightplanForCallsign(flightPlan.GetCallsign()).
                SetActualOffBlockTime(std::chrono::system_clock::now());
        }

        std::string ActualOffBlockTimeEventHandler::GetTagItemDescription(void) const
        {
            return "Actual Off-block Time";
        }

        std::string ActualOffBlockTimeEventHandler::GetTagItemData(
            EuroScopeCFlightPlanInterface & flightPlan,
            EuroScopeCRadarTargetInterface & radarTarget
        ) {
            if (!this->flightplans.HasFlightplanForCallsign(flightPlan.GetCallsign())) {
                return this->defaultTime;
            }

            std::chrono::system_clock::time_point offBlock = this->flightplans.GetFlightplanForCallsign(
                flightPlan.GetCallsign()
            )
                .GetActualOffBlockTime();

            return offBlock == (std::chrono::system_clock::time_point::max)() ? this->defaultTime
                : this->displayTime.FromTimePoint(offBlock);
        }
    }  // namespace Datablock
}  // namespace UKControllerPlugin
