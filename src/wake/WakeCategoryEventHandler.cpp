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
            return this->tagItemDescriptions.count(tagItemId)
                ? this->tagItemDescriptions.at(tagItemId)
                : "";
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

        std::string WakeCategoryEventHandler::GetMappedCategory(
            const WakeCategoryMapper& mapper,
            const std::string aircraftType,
            const std::string defaultValue
        ) const {
            std::string mapping = mapper.GetCategoryForAircraftType(aircraftType);
            return mapping == mapper.noCategory ? defaultValue : mapping;
        }

        /*
         * Get the data for the combined Aircraft Type / UK Category item.
         */
        std::string WakeCategoryEventHandler::GetAircraftTypeUkCategoryTagItemData(TagData& tagData)
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
        std::string WakeCategoryEventHandler::GetAircraftTypeRecatCategoryTagItemData(TagData& tagData)
        {
            CacheItem& cached = this->FirstOrNewCacheItem(tagData.flightPlan.GetCallsign());
            if (cached.aircraftTypeRecatCategoryItem == cached.noData)
            {
                cached.aircraftTypeRecatCategoryItem = this->GetAircraftTypeCategoryString(
                    tagData.flightPlan.GetAircraftType(),
                    this->recatMapper,
                    this->unknownTagItemString
                );
            }

            return cached.aircraftTypeRecatCategoryItem;
        }

        /*
         * Get the data for the standalone category item.
         */
        std::string WakeCategoryEventHandler::GetStandaloneTagItemData(TagData& tagData)
        {
            CacheItem& cached = this->FirstOrNewCacheItem(tagData.flightPlan.GetCallsign());
            if (cached.standaloneItem == cached.noData)
            {
                cached.standaloneItem = this->GetMappedCategory(
                    this->ukMapper,
                    tagData.flightPlan.GetAircraftType(),
                    this->unknownTagItemString
                );
            }

            return cached.standaloneItem;
        }

        /*
         * Get the data for the RECAT-EU category item.
         */
        std::string WakeCategoryEventHandler::GetRecatTagItemData(TagData& tagData)
        {
            CacheItem& cached = this->FirstOrNewCacheItem(tagData.flightPlan.GetCallsign());
            if (cached.recatItem == cached.noData)
            {
                cached.recatItem = this->GetMappedCategory(
                    this->recatMapper,
                    tagData.flightPlan.GetAircraftType(),
                    this->unknownTagItemString
                );
            }

            return cached.recatItem;
        }

        /*
         * Return a combination of UK and RECAT categories
         */
        std::string WakeCategoryEventHandler::GetUkRecatCombinedTagItemData(TagData& tagData)
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
            std::string mappedCategory = this->GetMappedCategory(
                mapper,
                aircraftType,
                defaultValue
            );

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
