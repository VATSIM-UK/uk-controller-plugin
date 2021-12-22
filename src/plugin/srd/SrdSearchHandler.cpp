#include "dialog/DialogManager.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "srd/SrdSearchHandler.h"
#include "srd/SrdSearchParameters.h"

using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Plugin::PopupMenuItem;

namespace UKControllerPlugin::Srd {

    SrdSearchHandler::SrdSearchHandler(const int menuCallbackId, const DialogManager& dialog)
        : dialog(dialog), menuCallbackId(menuCallbackId)
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
        if (flightplan.GetOrigin() == "") {
            dialog.OpenDialog(IDD_SRD_SEARCH, NULL);
        } else {
            this->lastSearchParameters = std::shared_ptr<SrdSearchParameters>(new SrdSearchParameters{
                flightplan.GetOrigin(),
                flightplan.GetDestination(),
                static_cast<unsigned int>(flightplan.GetCruiseLevel()),
                flightplan.GetCallsign()});
            dialog.OpenDialog(IDD_SRD_SEARCH, reinterpret_cast<LPARAM>(this->lastSearchParameters.get()));
        }
    }
} // namespace UKControllerPlugin::Srd
