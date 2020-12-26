#pragma once
#include "wake/WakeCategoryMapper.h"
#include "flightplan/FlightPlanEventHandlerInterface.h"
#include "tag/TagItemInterface.h"
#include "tag/TagData.h"
#include "wake/CacheItem.h"

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
                const int tagItemIdStandaloneCategory = 112;
                const int tagItemIdRecat = 113;
                const int tagItemIdUkRecatCombined = 114;

            private:

                std::string GetAircraftTypeTagItemData(UKControllerPlugin::Tag::TagData& tagData);
                std::string GetStandaloneTagItemData(UKControllerPlugin::Tag::TagData& tagData);
                std::string GetRecatTagItemData(UKControllerPlugin::Tag::TagData& tagData);
                std::string GetUkRecatCombinedTagItemData(UKControllerPlugin::Tag::TagData& tagData);
                CacheItem & FirstOrNewCacheItem(const std::string callsign);
                

                // The maximum length we can have in a tag item
                const size_t maxItemSize = 15;

                // Cache any data
                std::map<std::string, CacheItem> cache;

                // Maps categories
                const UKControllerPlugin::Wake::WakeCategoryMapper mapper;

                // Maps recat categories
                const UKControllerPlugin::Wake::WakeCategoryMapper recatMapper;
        };

    }  // namespace Wake
}  // namespace UKControllerPlugin
