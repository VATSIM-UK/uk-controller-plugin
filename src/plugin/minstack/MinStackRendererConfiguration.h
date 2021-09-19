#pragma once
#include "minstack/MinStackRenderedItem.h"

namespace UKControllerPlugin::MinStack {

    /*
        A class that manages the order and
        contents of the minstack display for a given
        rendered instance.
    */
    class MinStackRendererConfiguration
    {
        public:
        void AddItem(const UKControllerPlugin::MinStack::MinStackRenderedItem& item);
        [[nodiscard]] auto CountItems() const -> size_t;
        [[nodiscard]] auto GetItem(const std::string& key) const -> UKControllerPlugin::MinStack::MinStackRenderedItem;
        void RemoveItem(const UKControllerPlugin::MinStack::MinStackRenderedItem& item);
        void RemoveItem(int index);
        void Reset();
        void SetShouldRender(bool shouldRender);
        [[nodiscard]] auto ShouldRender() const -> bool;
        [[nodiscard]] auto InvalidItem() const -> const MinStackRenderedItem&;

        // Public type definitions for a custom iterator over the class.
        using MinStackRenderedItems = std::set<UKControllerPlugin::MinStack::MinStackRenderedItem>;
        using const_iterator = MinStackRenderedItems::const_iterator;
        using iterator = MinStackRenderedItems::iterator;
        [[nodiscard]] auto cbegin() const -> const_iterator
        {
            return this->items.cbegin();
        }
        [[nodiscard]] auto cend() const -> const_iterator
        {
            return this->items.cend();
        }
        [[nodiscard]] auto begin() const -> iterator
        {
            return this->items.begin();
        }
        [[nodiscard]] auto end() const -> iterator
        {
            return this->items.end();
        }

        private:
        // The invalid item
        const UKControllerPlugin::MinStack::MinStackRenderedItem invalidItem = {-9999, ""};

        // The items to render
        std::set<UKControllerPlugin::MinStack::MinStackRenderedItem> items;

        // Should the MinStacks be rendered?
        bool shouldRender = true;
    };
} // namespace UKControllerPlugin::MinStack
