#pragma once
#include "pch/stdafx.h"
#include "minstack/MinStackRendererConfiguration.h"

namespace UKControllerPlugin {
    namespace MinStack {

        void MinStackRendererConfiguration::AddItem(const MinStackRenderedItem item)
        {
            if (!this->items.insert(item).second) {
                LogWarning("Attempted to add duplicate item: " + item.key);
            }
        }

        size_t MinStackRendererConfiguration::CountItems(void) const
        {
            return this->items.size();
        }

        void MinStackRendererConfiguration::RemoveItem(const MinStackRenderedItem item)
        {
            this->items.erase(item);
        }

        void MinStackRendererConfiguration::RemoveItem(unsigned int index)
        {
            auto item = std::find_if(
                this->items.begin(),
                this->items.end(),
                [index](const MinStackRenderedItem & item) -> bool {
                    return item.order == index;
                }
            );

            if (item != this->items.end()) {
                this->items.erase(item);
            }
        }

        void MinStackRendererConfiguration::Reset(void)
        {
            this->items.clear();
        }
    }  // namespace MinStack
}  // namespace UKControllerPlugin
