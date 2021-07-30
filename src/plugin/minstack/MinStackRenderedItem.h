#pragma once
#include "pch/pch.h"

namespace UKControllerPlugin {
    namespace MinStack {

        typedef struct MinStackRenderedItem
        {
            // The order in which to render the item
            const unsigned int order;

            // The min stack key to find the actual level
            const std::string key;

            bool operator<(const MinStackRenderedItem & compare) const
            {
                return this->order < compare.order;
            }

            bool operator==(const MinStackRenderedItem & compare) const
            {
                return this->order == compare.order
                    && this->key == compare.key;
            }

            bool operator!=(const MinStackRenderedItem & compare) const
            {
                return !(*this == compare);
            }
        } MinStackRenderedItem;
    }  // namespace MinStack
}  // namespace UKControllerPlugin
