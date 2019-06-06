#pragma once
#include "tag/TagItemInterface.h"
#include "datablock/DisplayTime.h"
#include "flightplan/FlightPlanEventHandlerInterface.h"

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroScopeCFlightPlanInterface;
        class EuroScopeCRadarTargetInterface;
    }  // namespace Euroscope
    namespace Flightplan {
        class StoredFlightplanCollection;
    }  // namespace Flightplan
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
namespace Datablock {

class EstimatedDepartureTimeEventHandler : public UKControllerPlugin::Tag::TagItemInterface,
    public UKControllerPlugin::Flightplan::FlightPlanEventHandlerInterface
{
    public:
        EstimatedDepartureTimeEventHandler(
            const UKControllerPlugin::Flightplan::StoredFlightplanCollection & storedFlightplans,
            const UKControllerPlugin::Datablock::DisplayTime & displayTime
        );
        void FlightPlanEvent(
            UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightPlan,
            UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface & radarTarget
        ) override;
        void FlightPlanDisconnectEvent(
            UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightPlan
        ) override;
        void ControllerFlightPlanDataEvent(
            UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightPlan, int dataType
        ) override;
        std::string GetTagItemDescription(void) const override;
        std::string GetTagItemData(
            UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightPlan,
            UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface & radarTarget
        ) override;

    private:

        // Stored flightplans
        const UKControllerPlugin::Flightplan::StoredFlightplanCollection & storedFlightplans;

        // Class for converting timestamps to display times and getting the blank time format
        const UKControllerPlugin::Datablock::DisplayTime & displayTime;
};
}  // namespace Datablock
}  // namespace UKControllerPlugin
