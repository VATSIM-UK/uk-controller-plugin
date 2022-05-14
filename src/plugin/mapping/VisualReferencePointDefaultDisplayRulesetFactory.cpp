#include "VisualReferencePointDefaultDisplayRulesetFactory.h"

namespace UKControllerPlugin::Mapping {

    auto VisualReferencePointDefaultDisplayRulesetFactory::Make(
        const std::string& elementType, const nlohmann::json& data) const -> std::set<std::shared_ptr<DisplayRule>>
    {
        return nullptr;
    }
} // namespace UKControllerPlugin::Mapping
