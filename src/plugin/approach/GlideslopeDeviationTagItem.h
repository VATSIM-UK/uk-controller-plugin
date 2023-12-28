#pragma once
#include "tag/TagItemInterface.h"

namespace UKControllerPlugin {
    namespace Runway {
        class RunwayCollection;
    } // namespace Runway
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Approach {

    class GlideslopeDeviationEstimator;

    class GlideslopeDeviationTagItem : public Tag::TagItemInterface
    {
        public:
        GlideslopeDeviationTagItem(
            std::shared_ptr<const GlideslopeDeviationEstimator> glideslopeDeviationEstimator,
            std::shared_ptr<const Runway::RunwayCollection> runways);
        auto GetTagItemDescription(int tagItemId) const -> std::string override;
        void SetTagItemData(Tag::TagData& tagData) override;

        private:
        // The glideslope deviation estimator
        std::shared_ptr<const GlideslopeDeviationEstimator> glideslopeDeviationEstimator;

        // The runways
        std::shared_ptr<const Runway::RunwayCollection> runways;
    };

} // namespace UKControllerPlugin::Approach
