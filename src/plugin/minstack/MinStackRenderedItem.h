#pragma once

namespace UKControllerPlugin::MinStack {

    using MinStackRenderedItem = struct MinStackRenderedItem
    {
        // The order in which to render the item
        const unsigned int order;

        // The min stack key to find the actual level
        const std::string key;

        [[nodiscard]] auto operator<(const MinStackRenderedItem& compare) const -> bool;
        [[nodiscard]] auto operator==(const MinStackRenderedItem& compare) const -> bool;
        [[nodiscard]] auto operator!=(const MinStackRenderedItem& compare) const -> bool;
    };
} // namespace UKControllerPlugin::MinStack
