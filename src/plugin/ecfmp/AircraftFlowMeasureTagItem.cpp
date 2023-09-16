#include "AircraftFlowMeasureTagItem.h"
#include "AircraftFlowMeasureMapInterface.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "tag/TagData.h"

namespace UKControllerPlugin::ECFMP {

    AircraftFlowMeasureTagItem::AircraftFlowMeasureTagItem(
        std::shared_ptr<const AircraftFlowMeasureMapInterface> flowMeasureMap)
        : flowMeasureMap(std::move(flowMeasureMap))
    {
        assert(this->flowMeasureMap != nullptr && "Flow measure map cannot be null");
    }

    std::string AircraftFlowMeasureTagItem::GetTagItemDescription(int tagItemId) const
    {
        return "Relevant ECFMP Flow Measures";
    }

    void AircraftFlowMeasureTagItem::SetTagItemData(Tag::TagData& tagData)
    {
        const auto flowMeasuresForCallsign =
            flowMeasureMap->GetFlowMeasuresForCallsign(tagData.GetFlightplan().GetCallsign());
        if (flowMeasuresForCallsign.empty()) {
            return;
        }

        const auto measureCount =
            flowMeasuresForCallsign.size() > 1 ? "(" + std::to_string(flowMeasuresForCallsign.size()) + ")" : "";

        tagData.SetItemString(measureCount + (*flowMeasuresForCallsign.cbegin())->Identifier());
    }
} // namespace UKControllerPlugin::ECFMP
