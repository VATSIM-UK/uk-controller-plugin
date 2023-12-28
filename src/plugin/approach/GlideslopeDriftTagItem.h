#pragma once
#include "tag/TagItemInterface.h"

namespace UKControllerPlugin {
    namespace Runway {
        class RunwayCollection;
    } // namespace Runway
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Approach {

    class GlideslopeDriftEstimator;

    class GlideslopeDriftTagItem : public Tag::TagItemInterface
    {
        public:
        GlideslopeDriftTagItem(
            std::shared_ptr<const GlideslopeDriftEstimator> glideslopeDriftEstimator,
            std::shared_ptr<const Runway::RunwayCollection> runways);
        auto GetTagItemDescription(int tagItemId) const -> std::string override;
        void SetTagItemData(Tag::TagData& tagData) override;

        private:
        // The glideslope drift estimator
        std::shared_ptr<const GlideslopeDriftEstimator> glideslopeDriftEstimator;

        // The runways
        std::shared_ptr<const Runway::RunwayCollection> runways;
    };

} // namespace UKControllerPlugin::Approach
