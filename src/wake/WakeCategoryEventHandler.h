#pragma once
#include "wake/WakeCategoryMapper.h"
#include "flightplan/FlightPlanEventHandlerInterface.h"
#include "tag/TagItemInterface.h"

namespace UKControllerPlugin {
    namespace Wake {

        /*
            Handles wake category events
        */
        class WakeCategoryEventHandler : public UKControllerPlugin::Flightplan::FlightPlanEventHandlerInterface,
            public UKControllerPlugin::Tag::TagItemInterface
        {
            public:
                explicit WakeCategoryEventHandler(const UKControllerPlugin::Wake::WakeCategoryMapper mapper);
                void FlightPlanEvent(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightPlan,
                    UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface & radarTarget
                ) override;
                void FlightPlanDisconnectEvent(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightPlan
                ) override;
                void ControllerFlightPlanDataEvent(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightPlan,
                    int dataType
                ) override;
                std::string GetTagItemDescription(void) const override;
                std::string GetTagItemData(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightPlan,
                    UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface & radarTarget
                ) override;

            private:

                void RemoveCacheEntry(UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightPlan);

                // Cache any data
                std::map<std::string, std::string> cache;

                // Maps categories
                const UKControllerPlugin::Wake::WakeCategoryMapper mapper;
        };

    }  // namespace Wake
}  // namespace UKControllerPlugin
