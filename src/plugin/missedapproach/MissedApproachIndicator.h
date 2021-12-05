#pragma once
#include "tag/TagItemInterface.h"

namespace UKControllerPlugin::MissedApproach {
    class MissedApproachCollection;

    class MissedApproachIndicator : public Tag::TagItemInterface
    {
        public:
        MissedApproachIndicator(const MissedApproachCollection& missedApproaches);
        [[nodiscard]] auto GetTagItemDescription(int tagItemId) const -> std::string override;
        void SetTagItemData(Tag::TagData& tagData) override;

        private:
        const MissedApproachCollection& missedApproaches;
    };
} // namespace UKControllerPlugin::MissedApproach
