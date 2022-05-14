#pragma once
#include "DisplayRulesetFactoryInterface.h"

namespace UKControllerPlugin::Mapping {

    /**
     * Combines the data-driven and default display rulesets into one
     */
    class DisplayRulesetAggregatorFactory : public DisplayRulesetFactoryInterface
    {
        public:
        DisplayRulesetAggregatorFactory(
            const DisplayRulesetFactoryInterface& dataDrivenFactories,
            const DisplayRulesetFactoryInterface& defaultFactories);
        auto Make(const std::string& elementType, const nlohmann::json& data) const
            -> std::set<std::shared_ptr<DisplayRule>> override;

        // Factories that are driven specifically by the data
        const DisplayRulesetFactoryInterface& dataDrivenFactories;

        // Factories for default rules
        const DisplayRulesetFactoryInterface& defaultFactories;
    };
} // namespace UKControllerPlugin::Mapping
