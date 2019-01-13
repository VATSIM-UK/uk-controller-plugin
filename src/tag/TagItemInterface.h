#pragma once
// Forward declarations
namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroScopeCFlightPlanInterface;
        class EuroScopeCRadarTargetInterface;
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
// END

namespace UKControllerPlugin {
    namespace Tag {

        /*
            Interface for TagItems to follow. Provides facilities
            for calling the tag item and also providing a human readable description.
        */
        class TagItemInterface
        {
            public:
                virtual std::string GetTagItemDescription(void) const = 0;
                virtual std::string GetTagItemData(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightPlan,
                    UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface & radarTarget
                ) = 0;
        };
    }  // namespace Tag
}  // namespace UKControllerPlugin
