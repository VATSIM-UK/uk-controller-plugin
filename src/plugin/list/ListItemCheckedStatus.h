#pragma once

namespace UKControllerPlugin::List {
    /**
     * Helpful enum class around the ES list statuses.
     */
    enum class ListItemCheckedStatus : int
    {
        NoCheckbox = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX,
        NotChecked = EuroScopePlugIn::POPUP_ELEMENT_UNCHECKED,
        Checked = EuroScopePlugIn::POPUP_ELEMENT_CHECKED,
    };
} // namespace UKControllerPlugin::List
