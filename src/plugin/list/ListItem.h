#pragma once
#include "ListItemCheckedStatus.h"

namespace UKControllerPlugin::List {
    using ListItem = struct ListItem
    {
        ListItem(
            std::string firstColumn,
            std::string secondColumn,
            bool disabled,
            bool fixedPosition,
            ListItemCheckedStatus checked)
            : firstColumn(std::move(firstColumn)), secondColumn(std::move(secondColumn)), disabled(disabled),
              fixedPosition(fixedPosition), checked(checked)
        {
        }

        // First column value, used in the callback if selected
        std::string firstColumn;

        // Seconc column value, not required
        std::string secondColumn;

        // Is the item greyed out
        bool disabled;

        // Does the item have a fixed position
        bool fixedPosition;

        // Checkbox status
        ListItemCheckedStatus checked;
    };
} // namespace UKControllerPlugin::List
