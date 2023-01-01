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
        IntentionCodeTagItem(std::shared_ptr<AircraftIntentionCodeGenerator> intentionCodeGenerator);
        [[nodiscard]] auto GetTagItemDescription(int tagItemId) const -> std::string override;
        void SetTagItemData(Tag::TagData& tagData) override;

        private:
        // Generates intention codes
        std::shared_ptr<AircraftIntentionCodeGenerator> intentionCodeGenerator;
    };

} // namespace UKControllerPlugin::IntentionCode
