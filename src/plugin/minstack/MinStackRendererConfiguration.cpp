#include "MinStackRendererConfiguration.h"

namespace UKControllerPlugin::MinStack {

    void MinStackRendererConfiguration::AddItem(const MinStackRenderedItem& item)
    {
        if (!this->items.insert(item).second) {
            LogWarning("Attempted to add duplicate item: " + item.key);
        }
    }

    auto MinStackRendererConfiguration::CountItems() const -> size_t
    {
        return this->items.size();
    }

    auto MinStackRendererConfiguration::GetItem(const std::string& key) const -> MinStackRenderedItem
    {
        auto item =
            std::find_if(this->items.begin(), this->items.end(), [key](const MinStackRenderedItem& item) -> bool {
                return item.key == key;
            });

        return item == this->items.cend() ? this->invalidItem : *item;
    }

    void MinStackRendererConfiguration::RemoveItem(const MinStackRenderedItem& item)
    {
        this->items.erase(item);
    }

    void MinStackRendererConfiguration::RemoveItem(int index)
    {
        auto item =
            std::find_if(this->items.begin(), this->items.end(), [index](const MinStackRenderedItem& item) -> bool {
                return item.order == index;
            });

        if (item != this->items.end()) {
            this->items.erase(item);
        }
    }

    void MinStackRendererConfiguration::Reset()
    {
        this->items.clear();
    }

    void MinStackRendererConfiguration::SetShouldRender(bool shouldRender)
    {
        this->shouldRender = shouldRender;
    }

    auto MinStackRendererConfiguration::ShouldRender() const -> bool
    {
        return this->shouldRender;
    }

    auto MinStackRendererConfiguration::InvalidItem() const -> const MinStackRenderedItem&
    {
        return this->invalidItem;
    }
} // namespace UKControllerPlugin::MinStack
