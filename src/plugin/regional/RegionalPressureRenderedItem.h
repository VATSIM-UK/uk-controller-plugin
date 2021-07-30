#pragma once
#include "pch/pch.h"

namespace UKControllerPlugin {
    namespace Regional {

        typedef struct RegionalPressureRenderedItem
        {
            // The order in which to render the item
            const unsigned int order;

            // The key to find the actual rps
            const std::string key;

            bool operator<(const RegionalPressureRenderedItem& compare) const
            {
                return this->order < compare.order;
            }

            bool operator==(const RegionalPressureRenderedItem& compare) const
            {
                return this->order == compare.order
                    && this->key == compare.key;
            }

            bool operator!=(const RegionalPressureRenderedItem& compare) const
            {
                return !(*this == compare);
            }
        } RegionalPressureRenderedItem;
    }  // namespace Regional
}  // namespace UKControllerPlugin
