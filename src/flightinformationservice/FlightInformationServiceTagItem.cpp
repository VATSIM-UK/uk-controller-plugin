#include "pch/stdafx.h"
#include "flightinformationservice/FlightInformationServiceTagItem.h"

namespace UKControllerPlugin {
    namespace FlightInformationService {
        std::string FlightInformationServiceTagItem::GetTagItemDescription(int tagItemId) const
        {
            return tagItemId == this->flightInformationServiceTagItem
                       ? "Flight Information Service"
                       : "Flight Information Service or Blank";
        }

        void FlightInformationServiceTagItem::SetTagItemData(Tag::TagData& tagData)
        {
            const auto aircraftService = this->aircraftServiceMap.find(tagData.flightPlan.GetCallsign());
            tagData.SetItemString(
                aircraftService == this->aircraftServiceMap.cend()
                       ? this->GetNoServiceItemString(tagData.itemCode)
                       : aircraftService->second
            );
        }

        std::string FlightInformationServiceTagItem::GetServiceForAircraft(std::string aircraft) const
        {
            const auto aircraftService = this->aircraftServiceMap.find(aircraft);
            return aircraftService == this->aircraftServiceMap.cend()
                       ? ""
                       : aircraftService->second;
        }

        void FlightInformationServiceTagItem::SetServiceForAircraft(std::string aircraft, std::string service)
        {
            this->aircraftServiceMap[aircraft] = service;
        }

        void FlightInformationServiceTagItem::FlightPlanDisconnectEvent(
            Euroscope::EuroScopeCFlightPlanInterface& flightPlan
        )
        {
            this->aircraftServiceMap.erase(flightPlan.GetCallsign());
        }

        std::string FlightInformationServiceTagItem::GetNoServiceItemString(int tagItemId) const
        {
            return tagItemId == this->flightInformationServiceTagItem ? "UKFIS" : "";
        }
    }  // namespace FlightInformationService
}  // namespace UKControllerPlugin
