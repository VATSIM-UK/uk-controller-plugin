#include "ConfigureMissedApproaches.h"
#include "MissedApproachRenderOptions.h"
#include "dialog/DialogManager.h"

using UKControllerPlugin::Plugin::PopupMenuItem;

namespace UKControllerPlugin::MissedApproach {
    ConfigureMissedApproaches::ConfigureMissedApproaches(
        std::shared_ptr<MissedApproachRenderOptions> renderOptions,
        UKControllerPlugin::Dialog::DialogManager& dialogManager,
        int configureCallbackId)
        : renderOptions(std::move(renderOptions)), dialogManager(dialogManager),
          configureCallbackId(configureCallbackId)
    {
    }

    void ConfigureMissedApproaches::Configure(int functionId, std::string subject, RECT screenObjectArea)
    {
        this->dialogManager.OpenDialog(IDD_MISSED_APPROACH, reinterpret_cast<LPARAM>(&renderOptions)); // NOLINT
    }

    auto ConfigureMissedApproaches::GetConfigurationMenuItem() const -> UKControllerPlugin::Plugin::PopupMenuItem
    {
        PopupMenuItem returnVal;
        returnVal.firstValue = "Configure Missed Approaches";
        returnVal.secondValue = "";
        returnVal.callbackFunctionId = this->configureCallbackId;
        returnVal.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
        returnVal.disabled = false;
        returnVal.fixedPosition = false;
        return returnVal;
    }
} // namespace UKControllerPlugin::MissedApproach
