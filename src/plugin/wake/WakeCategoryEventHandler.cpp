#include "CacheItem.h"
#include "WakeCategory.h"
#include "WakeCategoryEventHandler.h"
#include "WakeCategoryMapperInterface.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "tag/TagData.h"

using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Tag::TagData;

namespace UKControllerPlugin::Wake {

    WakeCategoryEventHandler::WakeCategoryEventHandler(
        WakeCategoryMapperInterface& ukMapper, WakeCategoryMapperInterface& recatMapper)
        : ukMapper(ukMapper), recatMapper(recatMapper)
    {
    }

    /*
        Flightplan has changed, clear the cache
    */
    void WakeCategoryEventHandler::FlightPlanEvent(
        EuroScopeCFlightPlanInterface& flightPlan, EuroScopeCRadarTargetInterface& radarTarget)
    {
        this->cache.erase(flightPlan.GetCallsign());
    }

    /*
        Flightplan has gone, clear the cache
    */
    void WakeCategoryEventHandler::FlightPlanDisconnectEvent(EuroScopeCFlightPlanInterface& flightPlan)
    {
        this->cache.erase(flightPlan.GetCallsign());
    }

    /*
        Nothing to do here
    */
    void
    WakeCategoryEventHandler::ControllerFlightPlanDataEvent(EuroScopeCFlightPlanInterface& flightPlan, int dataType)
    {
    }

    auto WakeCategoryEventHandler::GetTagItemDescription(int tagItemId) const -> std::string
    {
        return this->tagItemDescriptions.count(tagItemId) == 1 ? this->tagItemDescriptions.at(tagItemId) : "";
    }

    /*
        Get item from the cache or generate if not exists
    */
    void WakeCategoryEventHandler::SetTagItemData(TagData& tagData)
    {
        const auto& itemCode = tagData.GetItemCode();
        if (itemCode == tagItemIdAircraftTypeCategory) {
            tagData.SetItemString(this->GetAircraftTypeUkCategoryTagItemData(tagData));
        } else if (itemCode == tagItemIdStandaloneCategory) {
            tagData.SetItemString(this->GetStandaloneTagItemData(tagData));
        } else if (itemCode == tagItemIdRecat) {
            tagData.SetItemString(this->GetRecatTagItemData(tagData));
        } else if (itemCode == tagItemIdUkRecatCombined) {
            tagData.SetItemString(this->GetUkRecatCombinedTagItemData(tagData));
        } else if (itemCode == tagItemIdAircraftTypeRecat) {
            tagData.SetItemString(this->GetAircraftTypeRecatCategoryTagItemData(tagData));
        }
    }

    auto WakeCategoryEventHandler::GetMappedCategory(
        WakeCategoryMapperInterface& mapper,
        const Euroscope::EuroScopeCFlightPlanInterface& flightplan,
        const std::string& defaultValue) -> std::string
    {
        const auto mapping = mapper.MapForFlightplan(flightplan);
        return mapping == nullptr ? defaultValue : mapping->Code();
    }

    /*
     * Get the data for the combined Aircraft Type / UK Category item.
     */
    auto WakeCategoryEventHandler::GetAircraftTypeUkCategoryTagItemData(TagData& tagData) -> std::string
    {
        const auto& flightplan = tagData.GetFlightplan();
        CacheItem& cached = this->FirstOrNewCacheItem(flightplan.GetCallsign());
        if (cached.aircraftTypeUKCategoryItem == cached.noData) {
            cached.aircraftTypeUKCategoryItem =
                this->GetAircraftTypeCategoryString(flightplan, this->ukMapper, flightplan.GetIcaoWakeCategory());
        }

        return cached.aircraftTypeUKCategoryItem;
    }

    /*
     * Get the data for the combined Aircraft Type / RECAT-EU category item.
     */
    auto WakeCategoryEventHandler::GetAircraftTypeRecatCategoryTagItemData(TagData& tagData) -> std::string
    {
        const auto& flightplan = tagData.GetFlightplan();
        CacheItem& cached = this->FirstOrNewCacheItem(flightplan.GetCallsign());
        if (cached.aircraftTypeRecatCategoryItem == cached.noData) {
            cached.aircraftTypeRecatCategoryItem =
                this->GetAircraftTypeCategoryString(flightplan, this->recatMapper, this->unknownTagItemString);
        }

        return cached.aircraftTypeRecatCategoryItem;
    }

    /*
     * Get the data for the standalone category item.
     */
    auto WakeCategoryEventHandler::GetStandaloneTagItemData(TagData& tagData) -> std::string
    {
        const auto& flightplan = tagData.GetFlightplan();
        CacheItem& cached = this->FirstOrNewCacheItem(flightplan.GetCallsign());
        if (cached.standaloneItem == cached.noData) {
            cached.standaloneItem = this->GetMappedCategory(this->ukMapper, flightplan, this->unknownTagItemString);
        }

        return cached.standaloneItem;
    }

    /*
     * Get the data for the RECAT-EU category item.
     */
    auto WakeCategoryEventHandler::GetRecatTagItemData(TagData& tagData) -> std::string
    {
        const auto& flightplan = tagData.GetFlightplan();
        CacheItem& cached = this->FirstOrNewCacheItem(flightplan.GetCallsign());
        if (cached.recatItem == cached.noData) {
            cached.recatItem = this->GetMappedCategory(this->recatMapper, flightplan, this->unknownTagItemString);
        }

        return cached.recatItem;
    }

    /*
     * Return a combination of UK and RECAT categories
     */
    auto WakeCategoryEventHandler::GetUkRecatCombinedTagItemData(TagData& tagData) -> std::string
    {
        return this->GetStandaloneTagItemData(tagData) + "/" + this->GetRecatTagItemData(tagData);
    }

    /*
     * Create an aircraft type / wake category string, using a default fallback value if there is no mapped
     * category.
     */
    auto WakeCategoryEventHandler::GetAircraftTypeCategoryString(
        const Euroscope::EuroScopeCFlightPlanInterface& flightplan,
        WakeCategoryMapperInterface& mapper,
        const std::string& defaultValue) const -> std::string
    {
        auto aircraftType = flightplan.GetAircraftType();
        auto mappedCategory = this->GetMappedCategory(mapper, flightplan, defaultValue);
        auto tagString = aircraftType + "/" + mappedCategory;

        // 15 characters is the max for tag functions, trim the aircraft type accordingly
        if (tagString.size() > this->maxItemSize) {
            const unsigned int charactersToTrim = tagString.size() - this->maxItemSize;
            tagString = aircraftType.substr(0, aircraftType.size() - charactersToTrim) + "/" + mappedCategory;
        }

        return tagString;
    }

    /*
     * Return a fresh cache item, or the current item if it exists.
     */
    auto WakeCategoryEventHandler::FirstOrNewCacheItem(const std::string& callsign) -> CacheItem&
    {
        if (this->cache.count(callsign) == 0) {
            this->cache[callsign] = {};
        }

        return this->cache.at(callsign);
    }
} // namespace UKControllerPlugin::Wake
