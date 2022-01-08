#include "DepartureCoordinationList.h"
#include "ToggleDepartureCoordinationList.h"

namespace UKControllerPlugin::Departure {

    ToggleDepartureCoordinationList::ToggleDepartureCoordinationList(
        std::shared_ptr<DepartureCoordinationList> list, int callbackId)
        : list(list), callbackId(callbackId)
    {
    }

    void ToggleDepartureCoordinationList::Configure(int functionId, std::string subject, RECT screenObjectArea)
    {
        this->list->ToggleVisible();
    }

    auto ToggleDepartureCoordinationList::GetConfigurationMenuItem() const -> Plugin::PopupMenuItem
    {
        return {
            "Toggle Departure Coordination List",
            "",
            this->callbackId,
            this->list->IsVisible() ? EuroScopePlugIn::POPUP_ELEMENT_CHECKED : EuroScopePlugIn::POPUP_ELEMENT_UNCHECKED,
            false,
            false};
    }
} // namespace UKControllerPlugin::Departure
