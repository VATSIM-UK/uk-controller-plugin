#include "DisplayRulesetAggregatorFactory.h"

namespace UKControllerPlugin::Mapping {

    DisplayRulesetAggregatorFactory::DisplayRulesetAggregatorFactory(
        const DisplayRulesetFactoryInterface& dataDrivenFactories,
        const DisplayRulesetFactoryInterface& defaultFactories)
        : dataDrivenFactories(dataDrivenFactories), defaultFactories(defaultFactories)
    {
    }

    auto DisplayRulesetAggregatorFactory::Make(const std::string& elementType, const nlohmann::json& data) const
        -> std::set<std::shared_ptr<DisplayRule>>
    {
        auto dataDrivenRules = dataDrivenFactories.Make(elementType, data);
        auto defaultRules = defaultFactories.Make(elementType, data);
        std::set<std::shared_ptr<DisplayRule>> rules;
        rules.insert(dataDrivenRules.cbegin(), dataDrivenRules.cend());
        rules.insert(defaultRules.cbegin(), defaultRules.cend());

        return rules;
    }
} // namespace UKControllerPlugin::Mapping
