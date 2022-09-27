#pragma once
#include "DisplayRuleFactoryInterface.h"

namespace UKControllerPlugin::Mapping {

    /**
     * Delegates the building of display rule based on the type of element.
     */
    class DisplayRuleElementTypeDelegatorFactory : public DisplayRuleFactoryInterface
    {
        public:
        DisplayRuleElementTypeDelegatorFactory(std::map<std::string, std::shared_ptr<DisplayRuleFactoryInterface>> factories);
        auto Make(const std::string& elementType, const nlohmann::json& data) -> std::shared_ptr<DisplayRule> override;
        
        private:
        // All the factories
        const std::map<std::string, std::shared_ptr<DisplayRuleFactoryInterface>> factories;
    };
} // namespace UKControllerPlugin::Mapping
