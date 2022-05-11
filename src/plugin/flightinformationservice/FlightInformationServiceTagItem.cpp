#include "FlightInformationServiceTagItem.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "tag/TagData.h"

namespace UKControllerPlugin::FlightInformationService {
    FlightInformationServiceTagItem::FlightInformationServiceTagItem(
        Euroscope::EuroscopePluginLoopbackInterface& plugin, int callbackId)
        : callbackId(callbackId), plugin(plugin)
    {
    }

    std::string FlightInformationServiceTagItem::GetTagItemDescription(int tagItemId) const
    {
        return tagItemId == this->flightInformationServiceTagItem ? "Flight Information Service"
                                                                  : "Flight Information Service or Blank";
    }

    void FlightInformationServiceTagItem::SetTagItemData(Tag::TagData& tagData)
    {
        const auto aircraftService = this->aircraftServiceMap.find(tagData.GetFlightplan().GetCallsign());
        tagData.SetItemString(
            aircraftService == this->aircraftServiceMap.cend() ? this->GetNoServiceItemString(tagData.GetItemCode())
                                                               : aircraftService->second);
    }

    std::string FlightInformationServiceTagItem::GetServiceForAircraft(std::string aircraft) const
    {
        const auto aircraftService = this->aircraftServiceMap.find(aircraft);
        return aircraftService == this->aircraftServiceMap.cend() ? "" : aircraftService->second;
    }

    void FlightInformationServiceTagItem::SetServiceForAircraft(std::string aircraft, std::string service)
    {
        this->aircraftServiceMap[aircraft] = service;
    }

    void
    FlightInformationServiceTagItem::FlightPlanDisconnectEvent(Euroscope::EuroScopeCFlightPlanInterface& flightPlan)
    {
        this->aircraftServiceMap.erase(flightPlan.GetCallsign());
    }

    std::string FlightInformationServiceTagItem::GetNoServiceItemString(int tagItemId) const
    {
        return tagItemId == this->flightInformationServiceTagItem ? "UKFIS" : "";
    }

    void FlightInformationServiceTagItem::DisplayFlightInformationServicesMenu(
        Euroscope::EuroScopeCFlightPlanInterface& flightplan,
        Euroscope::EuroScopeCRadarTargetInterface& radarTarget,
        std::string context,
        const POINT& mousePos)
    {
        // Create the list in place
        RECT popupArea = {mousePos.x, mousePos.y, mousePos.x + 300, mousePos.y + 500};

        this->plugin.TriggerPopupList(popupArea, "UKFIS", 2);

        std::map<std::string, std::string> itemsToAdd{
            {"BS", "Basic"},
            {"TS", "Traffic"},
            {"DS", "Deconfliction"},
            {"TS", "Procedural"},
            {noUkFisSelected, "None"}};

        // Use a "different" callback function for each hold, so we can easily determine which one is called
        Plugin::PopupMenuItem menuItem;
        menuItem.callbackFunctionId = this->callbackId;
        menuItem.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
        menuItem.disabled = false;
        menuItem.fixedPosition = false;

        std::for_each(itemsToAdd.cbegin(), itemsToAdd.cend(), [this, &menuItem](auto item) {
            menuItem.firstValue = item.first;
            menuItem.secondValue = item.second;
            this->plugin.AddItemToPopupList(menuItem);
        });
    }

    void FlightInformationServiceTagItem::MenuItemClicked(int functionId, std::string context)
    {
        std::shared_ptr<Euroscope::EuroScopeCFlightPlanInterface> fp = this->plugin.GetSelectedFlightplan();

        if (!fp) {
            LogWarning("Tried to assign UKFIS to non-existent flightplan");
            return;
        }

        if (context == this->noUkFisSelected) {
            this->aircraftServiceMap.erase(fp->GetCallsign());
            return;
        }

        this->aircraftServiceMap[fp->GetCallsign()] = context;
    }
} // namespace UKControllerPlugin::FlightInformationService
