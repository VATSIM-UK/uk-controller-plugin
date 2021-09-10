#include "MinStackRenderedItem.h"

namespace UKControllerPlugin::MinStack {
    auto MinStackRenderedItem::operator<(const MinStackRenderedItem& compare) const -> bool
    {
        return this->order < compare.order;
    }

    auto MinStackRenderedItem::operator==(const MinStackRenderedItem& compare) const -> bool
    {
        return this->order == compare.order && this->key == compare.key;
    }

    auto MinStackRenderedItem::operator!=(const MinStackRenderedItem& compare) const -> bool
    {
        return !(*this == compare);
    }
} // namespace UKControllerPlugin::MinStack
