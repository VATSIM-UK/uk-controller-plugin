#pragma once
#include "pch/stdafx.h"

namespace UKControllerPlugin {
    namespace Wake {

        typedef struct CacheItem
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
        } CacheItem;
    }  // namespace Wake
}  // namespace UKControllerPlugin
