#pragma once

namespace UKControllerPlugin::Wake {

    using CacheItem = struct CacheItem
    {
        // What's there if no data
        std::string noData = "NODATA";

        // UK Wake Category
        std::string standaloneItem = noData;

        // RECAT-EU category
        std::string recatItem = noData;

        // UK Wake Category + Aircraft Type
        std::string aircraftTypeUKCategoryItem = noData;

        // RECAT-EU Category + Aircraft Type
        std::string aircraftTypeRecatCategoryItem = noData;
    };
} // namespace UKControllerPlugin::Wake
