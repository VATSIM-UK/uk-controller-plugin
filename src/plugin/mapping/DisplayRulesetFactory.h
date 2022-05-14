#pragma once
#include "DisplayRulesetFactoryInterface.h"

namespace UKControllerPlugin::Mapping {
    class DisplayRule;

    /**
     * Creates a set of display rules
     */
    class DisplayRulesetFactory : public DisplayRulesetFactoryInterface
    {
        public:
        [[nodiscard]] auto Make(const std::string& elementType, const nlohmann::json& data) const
            -> std::set<std::shared_ptr<DisplayRule>> override;
    };

} // namespace UKControllerPlugin::Mapping
