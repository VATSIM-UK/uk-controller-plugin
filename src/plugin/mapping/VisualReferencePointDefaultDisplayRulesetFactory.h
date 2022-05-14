#pragma once
#include "DisplayRulesetFactoryInterface.h"

namespace UKControllerPlugin::Mapping {
    /**
     * Builds the default rulset for VRPS
     */
    class VisualReferencePointDefaultDisplayRulesetFactory : public DisplayRulesetFactoryInterface
    {
        public:
        [[nodiscard]] auto Make(const std::string& elementType, const nlohmann::json& data) const
            -> std::set<std::shared_ptr<DisplayRule>> override;
    };
} // namespace UKControllerPlugin::Mapping
