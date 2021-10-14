#include "MissedApproachButton.h"
#include "ToggleMissedApproachButton.h"

using UKControllerPlugin::Plugin::PopupMenuItem;

namespace UKControllerPlugin::MissedApproach {

    ToggleMissedApproachButton::ToggleMissedApproachButton(
        std::shared_ptr<MissedApproachButton> button, int configureCallbackId)
        : button(std::move(button)), configureCallbackId(configureCallbackId)
    {
    }

    void ToggleMissedApproachButton::Configure(int functionId, std::string subject, RECT screenObjectArea)
    {
        this->button->ToggleVisible();
    }

    auto ToggleMissedApproachButton::GetConfigurationMenuItem() const -> Plugin::PopupMenuItem
    {
        PopupMenuItem returnVal;
        returnVal.firstValue = "Toggle Missed Approach Button";
        returnVal.secondValue = "";
        returnVal.callbackFunctionId = this->configureCallbackId;
        returnVal.checked = this->button->IsVisible() ? EuroScopePlugIn::POPUP_ELEMENT_CHECKED
                                                      : EuroScopePlugIn::POPUP_ELEMENT_UNCHECKED;
        returnVal.disabled = false;
        returnVal.fixedPosition = false;
        return returnVal;
    }

} // namespace UKControllerPlugin::MissedApproach
