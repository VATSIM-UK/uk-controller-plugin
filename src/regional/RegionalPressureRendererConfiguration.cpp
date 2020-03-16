#pragma once
#include "pch/stdafx.h"
#include "regional/RegionalPressureRendererConfiguration.h"

namespace UKControllerPlugin {
    namespace Regional {

        void RegionalPressureRendererConfiguration::AddItem(const RegionalPressureRenderedItem item)
        {
            if (!this->items.insert(item).second) {
                LogWarning("Attempted to add duplicate item: " + item.key);
            }
        }

        size_t RegionalPressureRendererConfiguration::CountItems(void) const
        {
            return this->items.size();
        }

        RegionalPressureRenderedItem RegionalPressureRendererConfiguration::GetItem(std::string key) const
        {
            auto item = std::find_if(
                this->items.begin(),
                this->items.end(),
                [key](const RegionalPressureRenderedItem& item) -> bool {
                    return item.key == key;
                }
            );

            return item == this->items.cend() ? this->invalidItem : *item;
        }

        void RegionalPressureRendererConfiguration::RemoveItem(const RegionalPressureRenderedItem item)
        {
            this->items.erase(item);
        }

        void RegionalPressureRendererConfiguration::RemoveItem(unsigned int index)
        {
            auto item = std::find_if(
                this->items.begin(),
                this->items.end(),
                [index](const RegionalPressureRenderedItem& item) -> bool {
                    return item.order == index;
                }
            );

            if (item != this->items.end()) {
                this->items.erase(item);
            }
        }

        void RegionalPressureRendererConfiguration::Reset(void)
        {
            this->items.clear();
        }

        void RegionalPressureRendererConfiguration::SetShouldRender(bool shouldRender)
        {
            this->shouldRender = shouldRender;
        }

        bool RegionalPressureRendererConfiguration::ShouldRender(void) const
        {
            return this->shouldRender;
        }
    }  // namespace Regional
}  // namespace UKControllerPlugin
