#include "PendingPrenoteList.h"
#include "TogglePendingPrenoteList.h"

namespace UKControllerPlugin::Prenote {

    TogglePendingPrenoteList::TogglePendingPrenoteList(std::shared_ptr<PendingPrenoteList> list, int callbackId)
        : list(std::move(list)), callbackId(callbackId)
    {
    }

    void TogglePendingPrenoteList::Configure(int functionId, std::string subject, RECT screenObjectArea)
    {
        this->list->ToggleVisible();
    }

    auto TogglePendingPrenoteList::GetConfigurationMenuItem() const -> Plugin::PopupMenuItem
    {
        return {
            "Toggle Pending Prenote List",
            "",
            this->callbackId,
            this->list->IsVisible() ? EuroScopePlugIn::POPUP_ELEMENT_CHECKED : EuroScopePlugIn::POPUP_ELEMENT_UNCHECKED,
            false,
            false};
    }
} // namespace UKControllerPlugin::Prenote
