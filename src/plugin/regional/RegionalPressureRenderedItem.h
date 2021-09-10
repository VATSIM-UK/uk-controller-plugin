#pragma once

namespace UKControllerPlugin::Regional {

    using RegionalPressureRenderedItem = struct RegionalPressureRenderedItem
    {
        // The order in which to render the item
        const unsigned int order;

        // The key to find the actual rps
        const std::string key;

        [[nodiscard]] auto operator<(const RegionalPressureRenderedItem& compare) const -> bool;
        [[nodiscard]] auto operator==(const RegionalPressureRenderedItem& compare) const -> bool;
        [[nodiscard]] auto operator!=(const RegionalPressureRenderedItem& compare) const -> bool;
    };
} // namespace UKControllerPlugin::Regional
