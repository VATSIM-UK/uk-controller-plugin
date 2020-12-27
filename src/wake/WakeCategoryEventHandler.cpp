#include "pch/stdafx.h"
#include "wake/WakeCategoryEventHandler.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"

using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Tag::TagData;

namespace UKControllerPlugin {
    namespace Wake {

        WakeCategoryEventHandler::WakeCategoryEventHandler(
            const WakeCategoryMapper ukMapper,
            const WakeCategoryMapper recatMapper
        )
            : ukMapper(ukMapper), recatMapper(recatMapper)
        {

        }

        /*
            Flightplan has changed, clear the cache
        */
        void WakeCategoryEventHandler::FlightPlanEvent(
            EuroScopeCFlightPlanInterface & flightPlan,
            EuroScopeCRadarTargetInterface & radarTarget
        ) {
            this->cache.erase(flightPlan.GetCallsign());
        }

        /*
            Flightplan has gone, clear the cache
        */
        void WakeCategoryEventHandler::FlightPlanDisconnectEvent(EuroScopeCFlightPlanInterface & flightPlan)
        {
            this->cache.erase(flightPlan.GetCallsign());
        }

        /*
            Nothing to do here
        */
        void WakeCategoryEventHandler::ControllerFlightPlanDataEvent(
            EuroScopeCFlightPlanInterface & flightPlan,
            int dataType
        ) {

        }

        std::string WakeCategoryEventHandler::GetTagItemDescription(int tagItemId) const
        {
            if (tagItemId == this->tagItemIdAircraftTypeCategory)
            {
                return "Aircraft Type / UK Wake Category";
            } else if (tagItemId == this->tagItemIdStandaloneCategory) {
                return "UK Wake Category";
            } else if (tagItemId ==  this->tagItemIdRecat) {
                return "RECAT-EU Category";
            } else if (tagItemId == this->tagItemIdUkRecatCombined) {
                return "UK / RECAT-EU Combined";
            } else if (tagItemId == this->tagItemIdAircraftTypeRecat) {
                return "Aircraft Type / RECAT-EU Category";
            }

            return "";
        }

        /*
            Get item from the cache or generate if not exists
        */
        void WakeCategoryEventHandler::SetTagItemData(TagData& tagData)
        {
            if (tagData.itemCode == this->tagItemIdAircraftTypeCategory) {
                tagData.SetItemString(this->GetAircraftTypeUkCategoryTagItemData(tagData));
            } else if (tagData.itemCode == this->tagItemIdStandaloneCategory) {
                tagData.SetItemString(this->GetStandaloneTagItemData(tagData));
            } else if (tagData.itemCode == this->tagItemIdRecat) {
                tagData.SetItemString(this->GetRecatTagItemData(tagData));
            } else if (tagData.itemCode == this->tagItemIdUkRecatCombined) {
                tagData.SetItemString(this->GetUkRecatCombinedTagItemData(tagData));
            } else if (tagData.itemCode == this->tagItemIdAircraftTypeRecat) {
                tagData.SetItemString(this->GetAircraftTypeRecatCategoryTagItemData(tagData));
            }
        }

        /*
         * Get the data for the combined Aircraft Type / UK Category item.
         */
        std::string WakeCategoryEventHandler::GetAircraftTypeUkCategoryTagItemData(UKControllerPlugin::Tag::TagData& tagData)
        {
            CacheItem& cached = this->FirstOrNewCacheItem(tagData.flightPlan.GetCallsign());
            if (cached.aircraftTypeUKCategoryItem == cached.noData)
            {
                cached.aircraftTypeUKCategoryItem = this->GetAircraftTypeCategoryString(
                    tagData.flightPlan.GetAircraftType(),
                    this->ukMapper,
                    tagData.flightPlan.GetIcaoWakeCategory()
                );
            }

            return cached.aircraftTypeUKCategoryItem;
        }

        /*
         * Get the data for the combined Aircraft Type / RECAT-EU category item.
         */
        std::string WakeCategoryEventHandler::GetAircraftTypeRecatCategoryTagItemData(UKControllerPlugin::Tag::TagData& tagData)
        {
            CacheItem& cached = this->FirstOrNewCacheItem(tagData.flightPlan.GetCallsign());
            if (cached.aircraftTypeRecatCategoryItem == cached.noData)
            {
                cached.aircraftTypeRecatCategoryItem = this->GetAircraftTypeCategoryString(
                    tagData.flightPlan.GetAircraftType(),
                    this->recatMapper,
                    "?"
                );
            }

            return cached.aircraftTypeRecatCategoryItem;
        }

        /*
         * Get the data for the standalone category item.
         */
        std::string WakeCategoryEventHandler::GetStandaloneTagItemData(UKControllerPlugin::Tag::TagData& tagData)
        {
            CacheItem& cached = this->FirstOrNewCacheItem(tagData.flightPlan.GetCallsign());
            if (cached.standaloneItem == cached.noData)
            {
                cached.standaloneItem = "?";
                cached.standaloneItem = this->ukMapper.GetCategoryForAircraftType(tagData.flightPlan.GetAircraftType());
            }

            return cached.standaloneItem;
        }

        /*
         * Get the data for the RECAT-EU category item.
         */
        std::string WakeCategoryEventHandler::GetRecatTagItemData(UKControllerPlugin::Tag::TagData& tagData)
        {
            CacheItem& cached = this->FirstOrNewCacheItem(tagData.flightPlan.GetCallsign());
            if (cached.recatItem == cached.noData)
            {
                cached.recatItem = "?";
                cached.recatItem = this->recatMapper.GetCategoryForAircraftType(tagData.flightPlan.GetAircraftType());
            }

            return cached.recatItem;
        }

        /*
         * Return a combination of UK and RECAT categories
         */
        std::string WakeCategoryEventHandler::GetUkRecatCombinedTagItemData(UKControllerPlugin::Tag::TagData& tagData)
        {
            return this->GetStandaloneTagItemData(tagData) + "/" + this->GetRecatTagItemData(tagData);
        }

        /*
         * Create an aircraft type / wake category string, using a default fallback value if there is no mapped
         * category.
         */
        std::string WakeCategoryEventHandler::GetAircraftTypeCategoryString(
            const std::string aircraftType,
            const WakeCategoryMapper& mapper,
            const std::string defaultValue
        ) const {
            std::string mappedCategory = mapper.GetCategoryForAircraftType(aircraftType);
            if (mappedCategory == mapper.noCategory)
            {
                mappedCategory = defaultValue;
            }

            std::string tagString = aircraftType + "/" + mappedCategory;

            // 15 characters is the max for tag functions, trim the aircraft type accordingly
            if (tagString.size() > this->maxItemSize) {
                const unsigned int charactersToTrim = tagString.size() - this->maxItemSize;
                tagString = aircraftType.substr(0, aircraftType.size() - charactersToTrim) + "/"
                    + mappedCategory;
            }

            return tagString;
        }

        /*
         * Return a fresh cache item, or the current item if it exists.
         */
        CacheItem& WakeCategoryEventHandler::FirstOrNewCacheItem(const std::string callsign)
        {
            if (!this->cache.count(callsign))
            {
                this->cache[callsign] = {};
            }

            return this->cache.at(callsign);
        }
    }  // namespace Wake
}  // namespace UKControllerPlugin
