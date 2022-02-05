#include "MenuToggleableDisplayInterface.h"
#include "ToggleDisplayFromMenu.h"
#include "list/ListItemCheckedStatus.h"

namespace UKControllerPlugin::RadarScreen {

    ToggleDisplayFromMenu::ToggleDisplayFromMenu(
        std::shared_ptr<MenuToggleableDisplayInterface> display, int callbackId)
        : display(std::move(display)), callbackId(callbackId)
    {
    }

    void ToggleDisplayFromMenu::Configure(int functionId, std::string subject, RECT screenObjectArea)
    {
        display->Toggle();
    }

    auto ToggleDisplayFromMenu::GetConfigurationMenuItem() const -> Plugin::PopupMenuItem
    {
        Plugin::PopupMenuItem item;
        item.firstValue = display->MenuItem();
        item.checked = static_cast<int>(
            display->IsVisible() ? List::ListItemCheckedStatus::Checked : List::ListItemCheckedStatus::NotChecked);
        item.disabled = false;
        item.fixedPosition = false;
        item.callbackFunctionId = callbackId;

        return item;
    }
} // namespace UKControllerPlugin::RadarScreen
