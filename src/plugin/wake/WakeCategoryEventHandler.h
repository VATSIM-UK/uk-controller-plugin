#pragma once
#include "CacheItem.h"
#include "WakeCategoryMapper.h"
#include "flightplan/FlightPlanEventHandlerInterface.h"
#include "tag/TagData.h"
#include "tag/TagItemInterface.h"

namespace UKControllerPlugin::Wake {

    /*
        Handles wake category events
    */
    class WakeCategoryEventHandler : public UKControllerPlugin::Flightplan::FlightPlanEventHandlerInterface,
                                     public UKControllerPlugin::Tag::TagItemInterface
    {
        public:
        explicit WakeCategoryEventHandler(
            UKControllerPlugin::Wake::WakeCategoryMapper ukMapper,
            UKControllerPlugin::Wake::WakeCategoryMapper recatMapper);
        void FlightPlanEvent(
            UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightPlan,
            UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& radarTarget) override;
        void
        FlightPlanDisconnectEvent(UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightPlan) override;
        void ControllerFlightPlanDataEvent(
            UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightPlan, int dataType) override;
        [[nodiscard]] auto GetTagItemDescription(int tagItemId) const -> std::string override;
        void SetTagItemData(UKControllerPlugin::Tag::TagData& tagData) override;
        
        // Tag item ids
        inline static const int tagItemIdAircraftTypeCategory = 105;
        inline static const int tagItemIdStandaloneCategory = 112;
        inline static const int tagItemIdRecat = 113;
        inline static const int tagItemIdUkRecatCombined = 114;
        inline static const int tagItemIdAircraftTypeRecat = 115;

        private:
        [[nodiscard]] static auto GetMappedCategory(
            const UKControllerPlugin::Wake::WakeCategoryMapper& mapper,
            const std::string& aircraftType,
            const std::string& defaultValue) -> std::string;
        [[nodiscard]] auto GetAircraftTypeUkCategoryTagItemData(UKControllerPlugin::Tag::TagData& tagData)
            -> std::string;
        ;
        [[nodiscard]] auto GetAircraftTypeRecatCategoryTagItemData(UKControllerPlugin::Tag::TagData& tagData)
            -> std::string;
        ;
        [[nodiscard]] auto GetStandaloneTagItemData(UKControllerPlugin::Tag::TagData& tagData) -> std::string;
        ;
        [[nodiscard]] auto GetRecatTagItemData(UKControllerPlugin::Tag::TagData& tagData) -> std::string;
        ;
        [[nodiscard]] auto GetUkRecatCombinedTagItemData(UKControllerPlugin::Tag::TagData& tagData) -> std::string;
        [[nodiscard]] auto GetAircraftTypeCategoryString(
            const std::string& aircraftType,
            const UKControllerPlugin::Wake::WakeCategoryMapper& mapper,
            const std::string& defaultValue) const -> std::string;
        auto FirstOrNewCacheItem(const std::string& callsign) -> CacheItem&;

        // The maximum length we can have in a tag item
        const size_t maxItemSize = 15;

        // Cache any data
        std::map<std::string, CacheItem> cache;

        // Maps categories
        const UKControllerPlugin::Wake::WakeCategoryMapper ukMapper;

        // Maps recat categories
        const UKControllerPlugin::Wake::WakeCategoryMapper recatMapper;

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
