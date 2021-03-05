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
        void FlightInformationServiceTagItem::SetTagItemData(Tag::TagData& tagData) {}
    }  // namespace FlightInformationService
}  // namespace UKControllerPlugin
