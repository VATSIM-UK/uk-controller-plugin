#include "CallsignSelectionList.h"
#include "CallsignSelectionProviderInterface.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "plugin/PopupMenuItem.h"

namespace UKControllerPlugin::Aircraft {

    CallsignSelectionList::CallsignSelectionList(
        std::shared_ptr<CallsignSelectionProviderInterface> callsignProvider,
        Euroscope::EuroscopePluginLoopbackInterface& plugin,
        int callbackFunctionId)
        : callsignProvider(std::move(callsignProvider)), plugin(plugin), callbackFunctionId(callbackFunctionId)
    {
    }

    void CallsignSelectionList::TriggerList(const POINT& location)
    {
        const auto callsigns = this->callsignProvider->GetCallsigns();
        if (callsigns.empty()) {
            return;
        }

        plugin.TriggerPopupList(RECT{location.x, location.y, location.x + 50, location.y + 100}, "Select Aircraft", 1);

        Plugin::PopupMenuItem menuItem;
        menuItem.secondValue = "";
        menuItem.callbackFunctionId = callbackFunctionId;
        menuItem.fixedPosition = false;
        menuItem.disabled = false;
        menuItem.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;

        for (const auto& callsign : callsigns) {
            menuItem.firstValue = callsign;
            plugin.AddItemToPopupList(menuItem);
        }
    }

    void CallsignSelectionList::CallsignSelected(const std::string& callsign)
    {
        callsignProvider->CallsignSelected(callsign);
    }
} // namespace UKControllerPlugin::Aircraft
