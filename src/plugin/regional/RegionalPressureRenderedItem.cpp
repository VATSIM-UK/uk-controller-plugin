#include "RegionalPressureRenderedItem.h"

namespace UKControllerPlugin::Regional {

    auto RegionalPressureRenderedItem::operator<(const RegionalPressureRenderedItem& compare) const -> bool
    {
        return this->order < compare.order;
    }

    auto RegionalPressureRenderedItem::operator==(const RegionalPressureRenderedItem& compare) const -> bool
    {
        return this->order == compare.order && this->key == compare.key;
    }

    auto RegionalPressureRenderedItem::operator!=(const RegionalPressureRenderedItem& compare) const -> bool
    {
        return !(*this == compare);
    }
} // namespace UKControllerPlugin::Regional
