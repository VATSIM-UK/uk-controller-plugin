#pragma once

namespace UKControllerPlugin::Mapping {
    class DisplayRulesetFactoryInterface;
    class MappingRenderOptions;

    class DisplayRulesetFactoryFactory
    {
        public:
        [[nodiscard]] auto Make(std::shared_ptr<MappingRenderOptions> renderOptions) const
            -> std::shared_ptr<DisplayRulesetFactoryInterface>;
    };
} // namespace UKControllerPlugin::Mapping
