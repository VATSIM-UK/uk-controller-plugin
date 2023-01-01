#pragma once
#include "tag/TagItemInterface.h"

namespace UKControllerPlugin::IntentionCode {

    class AircraftIntentionCodeGenerator;

    /**
     * Tag item to display an aircraft's intention code.
     */
    class IntentionCodeTagItem : public Tag::TagItemInterface
    {
        public:
        IntentionCodeTagItem(AircraftIntentionCodeGenerator& intentionCodeGenerator);
        [[nodiscard]] auto GetTagItemDescription(int tagItemId) const -> std::string override;
        void SetTagItemData(Tag::TagData& tagData) override;

        private:
        // Generates intention codes
        AircraftIntentionCodeGenerator& intentionCodeGenerator;
    };

} // namespace UKControllerPlugin::IntentionCode
