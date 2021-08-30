#pragma once
#include "tag/TagItemInterface.h"

namespace UKControllerPlugin::Prenote {
    class PrenoteMessageCollection;

    /*
     * Provides the prenote status indicator tag item
     */
    class PrenoteStatusIndicatorTagItem : public Tag::TagItemInterface
    {
        public:
        explicit PrenoteStatusIndicatorTagItem(std::shared_ptr<PrenoteMessageCollection> messages);
        [[nodiscard]] auto GetTagItemDescription(int tagItemId) const -> std::string override;
        void SetTagItemData(Tag::TagData& tagData) override;

        private:
        // All the messages
        const std::shared_ptr<PrenoteMessageCollection> messages;
    };
} // namespace UKControllerPlugin::Prenote
