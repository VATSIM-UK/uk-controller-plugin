#pragma once
#include <string>
#include "tag/TagItemInterface.h"

namespace UKControllerPlugin::ECFMP {

    class AircraftFlowMeasureMapInterface;

    class AircraftFlowMeasureTagItem : public Tag::TagItemInterface
    {
        public:
        AircraftFlowMeasureTagItem(std::shared_ptr<const AircraftFlowMeasureMapInterface> flowMeasureMap);
        auto GetTagItemDescription(int tagItemId) const -> std::string override;
        void SetTagItemData(Tag::TagData& tagData) override;

        private:
        // The map of flow measures
        std::shared_ptr<const AircraftFlowMeasureMapInterface> flowMeasureMap;
    };

} // namespace UKControllerPlugin::ECFMP
