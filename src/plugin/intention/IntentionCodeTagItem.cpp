#include "AircraftIntentionCode.h"
#include "AircraftIntentionCodeGenerator.h"
#include "IntentionCodeTagItem.h"
#include "tag/TagData.h"

namespace UKControllerPlugin::IntentionCode {

    IntentionCodeTagItem::IntentionCodeTagItem(std::shared_ptr<AircraftIntentionCodeGenerator> intentionCodeGenerator)
        : intentionCodeGenerator(intentionCodeGenerator)
    {
        assert(intentionCodeGenerator && "generator not set in IntentionCodeTagItem");
    }

    auto IntentionCodeTagItem::GetTagItemDescription(int tagItemId) const -> std::string
    {
        return "Intention Code";
    }

    void IntentionCodeTagItem::SetTagItemData(Tag::TagData& tagData)
    {
        const auto intentionCode = intentionCodeGenerator->Generate(tagData.GetFlightplan(), tagData.GetRadarTarget());
        tagData.SetItemString(intentionCode ? intentionCode->intentionCode : "--");
    }
} // namespace UKControllerPlugin::IntentionCode
