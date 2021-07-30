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
                    const UKControllerPlugin::Wake::WakeCategoryMapper ukMapper,
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

                // Tag item ids
                const int tagItemIdAircraftTypeCategory = 105;
                const int tagItemIdStandaloneCategory = 112;
                const int tagItemIdRecat = 113;
                const int tagItemIdUkRecatCombined = 114;
                const int tagItemIdAircraftTypeRecat = 115;

                // What to display in unknown conditions
                const std::string unknownTagItemString = "?";

            private:

                std::string GetMappedCategory(
                    const UKControllerPlugin::Wake::WakeCategoryMapper& mapper,
                    const std::string aircraftType,
                    const std::string defaultValue
                ) const;
                std::string GetAircraftTypeUkCategoryTagItemData(UKControllerPlugin::Tag::TagData& tagData);
                std::string GetAircraftTypeRecatCategoryTagItemData(UKControllerPlugin::Tag::TagData& tagData);
                std::string GetStandaloneTagItemData(UKControllerPlugin::Tag::TagData& tagData);
                std::string GetRecatTagItemData(UKControllerPlugin::Tag::TagData& tagData);
                std::string GetUkRecatCombinedTagItemData(UKControllerPlugin::Tag::TagData& tagData);
                std::string GetAircraftTypeCategoryString(
                    const std::string aircraftType,
                    const UKControllerPlugin::Wake::WakeCategoryMapper& mapper,
                    const std::string defaultValue
                ) const;
                CacheItem & FirstOrNewCacheItem(const std::string callsign);

                // The maximum length we can have in a tag item
                const size_t maxItemSize = 15;

                // Cache any data
                std::map<std::string, CacheItem> cache;

                // Maps categories
                const UKControllerPlugin::Wake::WakeCategoryMapper ukMapper;

                // Maps recat categories
                const UKControllerPlugin::Wake::WakeCategoryMapper recatMapper;
#
                // Maps the TAG item ids to descriptions
                const std::map<int, std::string> tagItemDescriptions = {
                    { tagItemIdAircraftTypeCategory, "Aircraft Type / UK Wake Category"},
                    { tagItemIdStandaloneCategory, "UK Wake Category"},
                    { tagItemIdRecat, "RECAT-EU Category"},
                    { tagItemIdUkRecatCombined, "UK / RECAT-EU Combined"},
                    { tagItemIdAircraftTypeRecat, "Aircraft Type / RECAT-EU Category"},
                };
        };

    }  // namespace Wake
}  // namespace UKControllerPlugin
