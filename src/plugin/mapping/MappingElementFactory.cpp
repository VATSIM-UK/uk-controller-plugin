#include "MappingElement.h"
#include "MappingElementDrawerFactory.h"
#include "MappingElementFactory.h"

namespace UKControllerPlugin::Mapping {

    MappingElementFactory::MappingElementFactory(const MappingElementDrawerFactory& drawerFactory)
        : drawerFactory(drawerFactory)
    {
    }

    auto MappingElementFactory::Make(const nlohmann::json& data) -> std::shared_ptr<MappingElement>
    {
        if (!DataIsValid(data)) {
            return nullptr;
        }

        const auto type = data.at("type").get<std::string>();
        if (!IsKnownType(type)) {
            LogWarning("Detected mapping element of unknown type, ignoring");
            return nullptr;
        }

        return std::make_shared<MappingElement>(
            data.at("id").get<int>(),
            data.at("type").get<std::string>(),
            data.at("name").get<std::string>(),
            std::set<std::shared_ptr<DisplayRule>>{},
            drawerFactory.Make(data));
    }

    /**
     * Checks that we have the minimum necessary data to build a mapping element.
     */
    auto MappingElementFactory::DataIsValid(const nlohmann::json& data) -> bool
    {
        return data.is_object() && data.contains("id") && data.at("id").is_number_integer() && data.contains("type") &&
               data.at("type").is_string() && data.contains("name") && data.at("name").is_string();
    }

    auto MappingElementFactory::IsKnownType(const std::string& type) -> bool
    {
        return type == "visual_reference_point";
    }
} // namespace UKControllerPlugin::Mapping
