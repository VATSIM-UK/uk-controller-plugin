#pragma once
#include "regional/RegionalPressureRenderedItem.h"

namespace UKControllerPlugin {
    namespace Regional {

        /*
            A class that manages the order and
            contents of the rps display for a given
            rendered instance.
        */
        class RegionalPressureRendererConfiguration
        {
            public:
                void AddItem(const UKControllerPlugin::Regional::RegionalPressureRenderedItem item);
                size_t CountItems(void) const;
                UKControllerPlugin::Regional::RegionalPressureRenderedItem GetItem(std::string key) const;
                void RemoveItem(const UKControllerPlugin::Regional::RegionalPressureRenderedItem item);
                void RemoveItem(unsigned int index);
                void Reset(void);
                void SetShouldRender(bool shouldRender);
                bool ShouldRender(void) const;

                // Public type definitions for a custom iterator over the class.
                typedef std::set<UKControllerPlugin::Regional::RegionalPressureRenderedItem>
                    RegionalPressureRenderedItems;
                typedef RegionalPressureRenderedItems::const_iterator const_iterator;
                const_iterator cbegin(void) const { return this->items.cbegin(); }
                const_iterator cend(void) const { return this->items.cend(); }

                const UKControllerPlugin::Regional::RegionalPressureRenderedItem invalidItem = { UINT_MAX, "" };

            private:

                // The items to render
                std::set<UKControllerPlugin::Regional::RegionalPressureRenderedItem> items;

                // Should the regionals be rendered?
                bool shouldRender = true;
        };

    }  // namespace Regional
}  // namespace UKControllerPlugin
