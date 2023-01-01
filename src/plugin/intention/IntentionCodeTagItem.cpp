#include "AircraftIntentionCode.h"
#include "AircraftIntentionCodeGenerator.h"
#include "IntentionCodeTagItem.h"
#include "tag/TagData.h"

namespace UKControllerPlugin::IntentionCode {

    IntentionCodeTagItem::IntentionCodeTagItem(AircraftIntentionCodeGenerator& intentionCodeGenerator)
        : intentionCodeGenerator(intentionCodeGenerator)
    {
    }

    auto IntentionCodeTagItem::GetTagItemDescription(int tagItemId) const -> std::string
    {
        return "UKCP Intention Code";
    }

    void IntentionCodeTagItem::SetTagItemData(Tag::TagData& tagData)
    {
        const auto intentionCode = intentionCodeGenerator.Generate(tagData.GetFlightplan(), tagData.GetRadarTarget());
        tagData.SetItemString(intentionCode ? intentionCode->intentionCode : "--");
    }
} // namespace UKControllerPlugin::IntentionCode
