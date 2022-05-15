#include "DisplayRulesetAggregatorFactory.h"
#include "DisplayRulesetFactoryFactory.h"

namespace UKControllerPlugin::Mapping {

    auto DisplayRulesetFactoryFactory::Make(std::shared_ptr<MappingRenderOptions> renderOptions) const
        -> std::shared_ptr<DisplayRulesetFactoryInterface>
    {
        assert(renderOptions && "Render options not set in DisplayRulesetFactoryFactory");
        
        return std::make_shared<DisplayRulesetAggregatorFactory>()
    }
} // namespace UKControllerPlugin
