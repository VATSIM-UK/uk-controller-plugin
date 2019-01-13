#pragma once
#include "tag/TagItemInterface.h"
#include "datablock/DisplayTime.h"

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

class EstimatedOffBlockTimeEventHandler : public UKControllerPlugin::Tag::TagItemInterface
{
    public:
        EstimatedOffBlockTimeEventHandler(
            const UKControllerPlugin::Flightplan::StoredFlightplanCollection & storedFlightplans
        );
        std::string GetTagItemDescription(void) const override;
        std::string GetTagItemData(
            UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightPlan,
            UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface & radarTarget
        ) override;

        // Value to return for tags when there is no EOBT
        const std::string noTime = "--:--";

    private:

        // Stored flightplans
        const UKControllerPlugin::Flightplan::StoredFlightplanCollection & storedFlightplans;

        // Class for converting timestamps to display times
        const UKControllerPlugin::Datablock::DisplayTime displayTime;
};
}  // namespace Datablock
}  // namespace UKControllerPlugin
