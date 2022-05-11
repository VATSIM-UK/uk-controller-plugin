#pragma once

namespace UKControllerPlugin::Mapping {
    class DisplayRule;

    /**
     * Builds display rules associated with mapping elements
     */
    class TypeSpecificDisplayRuleFactory
    {
        public:
        virtual ~TypeSpecificDisplayRuleFactory() = default;
        [[nodiscard]] virtual auto Make(const nlohmann::json& data) -> std::set<std::shared_ptr<DisplayRule>> = 0;
    };
} // namespace UKControllerPlugin::Mapping
