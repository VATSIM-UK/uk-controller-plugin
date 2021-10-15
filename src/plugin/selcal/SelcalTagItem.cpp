#include "ParsedSelcal.h"
#include "SelcalParser.h"
#include "SelcalTagItem.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "tag/TagData.h"

namespace UKControllerPlugin::Selcal {

    SelcalTagItem::SelcalTagItem(std::shared_ptr<SelcalParser> parser) : parser(std::move(parser))
    {
    }

    void SelcalTagItem::FlightPlanEvent(
        Euroscope::EuroScopeCFlightPlanInterface& flightPlan, Euroscope::EuroScopeCRadarTargetInterface& radarTarget)
    {
        this->cache.erase(flightPlan.GetCallsign());
    }

    void SelcalTagItem::FlightPlanDisconnectEvent(Euroscope::EuroScopeCFlightPlanInterface& flightPlan)
    {
        this->cache.erase(flightPlan.GetCallsign());
    }

    void
    SelcalTagItem::ControllerFlightPlanDataEvent(Euroscope::EuroScopeCFlightPlanInterface& flightPlan, int dataType)
    {
        // Nothing to do
    }

    auto SelcalTagItem::GetTagItemDescription(int tagItemId) const -> std::string
    {
        return tagItemId == SELCAL_TAG_ITEM_ID ? "SELCAL Code" : "SELCAL Code With Separator";
    }

    void SelcalTagItem::SetTagItemData(Tag::TagData& tagData)
    {
        if (this->cache.count(tagData.GetFlightplan().GetCallsign()) == 0) {
            this->cache[tagData.GetFlightplan().GetCallsign()] = this->parser->ParseFromString(tagData.GetFlightplan().GetRemarks());
        }

        tagData.SetItemString(this->GetFromCache(tagData.GetFlightplan().GetCallsign(), tagData.GetItemCode()));
    }

    auto SelcalTagItem::GetFromCache(const std::string& callsign, const int type) const -> std::string
    {
        const auto& cacheItem = this->cache.at(callsign);
        if (cacheItem == nullptr) {
            return "";
        }
        
        return type == SELCAL_TAG_ITEM_ID ? cacheItem->GetRaw() : cacheItem->GetWithSeparator();
    }
} // namespace UKControllerPlugin::Selcal
