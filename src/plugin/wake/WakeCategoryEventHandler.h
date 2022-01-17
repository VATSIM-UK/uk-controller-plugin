#pragma once
#include "CacheItem.h"
#include "flightplan/FlightPlanEventHandlerInterface.h"
#include "tag/TagItemInterface.h"

namespace UKControllerPlugin::Tag {
    class TagData;
} // namespace UKControllerPlugin::Tag

namespace UKControllerPlugin::Wake {
    class WakeCategoryMapperInterface;

    /*
        Handles wake category events
    */
    class WakeCategoryEventHandler : public UKControllerPlugin::Flightplan::FlightPlanEventHandlerInterface,
                                     public UKControllerPlugin::Tag::TagItemInterface
    {
        public:
        explicit WakeCategoryEventHandler(
            WakeCategoryMapperInterface& ukMapper, WakeCategoryMapperInterface& recatMapper);
        void FlightPlanEvent(
            Euroscope::EuroScopeCFlightPlanInterface& flightPlan,
            Euroscope::EuroScopeCRadarTargetInterface& radarTarget) override;
        void FlightPlanDisconnectEvent(Euroscope::EuroScopeCFlightPlanInterface& flightPlan) override;
        void ControllerFlightPlanDataEvent(Euroscope::EuroScopeCFlightPlanInterface& flightPlan, int dataType) override;
        [[nodiscard]] auto GetTagItemDescription(int tagItemId) const -> std::string override;
        void SetTagItemData(Tag::TagData& tagData) override;

        // Tag item ids
        inline static const int tagItemIdAircraftTypeCategory = 105;
        inline static const int tagItemIdStandaloneCategory = 112;
        inline static const int tagItemIdRecat = 113;
        inline static const int tagItemIdUkRecatCombined = 114;
        inline static const int tagItemIdAircraftTypeRecat = 115;

        private:
        [[nodiscard]] static auto GetMappedCategory(
            WakeCategoryMapperInterface& mapper,
            const Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            const std::string& defaultValue) -> std::string;
        [[nodiscard]] auto GetAircraftTypeUkCategoryTagItemData(Tag::TagData& tagData) -> std::string;
        [[nodiscard]] auto GetAircraftTypeRecatCategoryTagItemData(Tag::TagData& tagData) -> std::string;
        [[nodiscard]] auto GetStandaloneTagItemData(Tag::TagData& tagData) -> std::string;
        [[nodiscard]] auto GetRecatTagItemData(Tag::TagData& tagData) -> std::string;
        [[nodiscard]] auto GetUkRecatCombinedTagItemData(Tag::TagData& tagData) -> std::string;
        [[nodiscard]] auto GetAircraftTypeCategoryString(
            const Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            WakeCategoryMapperInterface& mapper,
            const std::string& defaultValue) const -> std::string;
        auto FirstOrNewCacheItem(const std::string& callsign) -> CacheItem&;

        // The maximum length we can have in a tag item
        const size_t maxItemSize = 15;

        // Cache any data
        std::map<std::string, CacheItem> cache;

        // Maps categories
        WakeCategoryMapperInterface& ukMapper;

        // Maps recat categories
        WakeCategoryMapperInterface& recatMapper;

        // What to display in unknown conditions
        const std::string unknownTagItemString = "?";

        // Maps the TAG item ids to descriptions
        const std::map<int, std::string> tagItemDescriptions = {
            {tagItemIdAircraftTypeCategory, "Aircraft Type / UK Wake Category"},
            {tagItemIdStandaloneCategory, "UK Wake Category"},
            {tagItemIdRecat, "RECAT-EU Category"},
            {tagItemIdUkRecatCombined, "UK / RECAT-EU Combined"},
            {tagItemIdAircraftTypeRecat, "Aircraft Type / RECAT-EU Category"},
        };
    };
} // namespace UKControllerPlugin::Wake
