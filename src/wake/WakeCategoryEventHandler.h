#pragma once
#include "wake/WakeCategoryMapper.h"
#include "flightplan/FlightPlanEventHandlerInterface.h"
#include "tag/TagItemInterface.h"
#include "tag/TagData.h"

namespace UKControllerPlugin {
    namespace Wake {

        /*
            Handles wake category events
        */
        class WakeCategoryEventHandler : public UKControllerPlugin::Flightplan::FlightPlanEventHandlerInterface,
            public UKControllerPlugin::Tag::TagItemInterface
        {
            public:
                explicit WakeCategoryEventHandler(
                    const UKControllerPlugin::Wake::WakeCategoryMapper mapper,
                    const UKControllerPlugin::Wake::WakeCategoryMapper recatMapper
                );
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
                std::string GetTagItemDescription(int tagItemId) const override;
                void SetTagItemData(UKControllerPlugin::Tag::TagData& tagData) override;

                const int tagItemIdAircraftTypeCategory = 105;
                const int tagItemIdRecat = 112;
                const int tagItemIdStandaloneCategory = 113;

            private:

                // The maximum length we can have in a tag item
                const size_t maxItemSize = 15;

                // Cache any data
                std::map<std::string, std::string> cache;

                // Maps categories
                const UKControllerPlugin::Wake::WakeCategoryMapper mapper;

                // Maps recat categories
                const UKControllerPlugin::Wake::WakeCategoryMapper recatMapper;
        };

    }  // namespace Wake
}  // namespace UKControllerPlugin
