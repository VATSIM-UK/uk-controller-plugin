#include "dialog/DialogManager.h"
#include "srd/SrdSearchHandler.h"

using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Plugin::PopupMenuItem;

namespace UKControllerPlugin::Srd {

    SrdSearchHandler::SrdSearchHandler(const int menuCallbackId, const DialogManager& dialog)
        : menuCallbackId(menuCallbackId), dialog(dialog)
    {
    }

    void SrdSearchHandler::Configure(int functionId, std::string subject, RECT screenObjectArea)
    {
        this->dialog.OpenDialog(IDD_SRD_SEARCH, NULL);
    }

    auto SrdSearchHandler::GetConfigurationMenuItem() const -> PopupMenuItem
    {
        PopupMenuItem returnVal;
        returnVal.firstValue = this->itemDescription;
        returnVal.secondValue = "";
        returnVal.callbackFunctionId = this->menuCallbackId;
        returnVal.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
        returnVal.disabled = false;
        returnVal.fixedPosition = false;
        return returnVal;
    }

    void SrdSearchHandler::TagFunction(
        EuroScopeCFlightPlanInterface& flightplan,
        EuroScopeCRadarTargetInterface& radarTarget,
        const std::string& context,
        const POINT& mousePos)
    {
        this->dialog.OpenDialog(IDD_SRD_SEARCH, NULL);
    }
} // namespace UKControllerPlugin::Srd
