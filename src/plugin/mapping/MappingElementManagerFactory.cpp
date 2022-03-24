#include "MappingElement.h"
#include "MappingElementManager.h"
#include "MappingElementManagerFactory.h"
#include "MappingModuleFactory.h"
#include "VisualReferencePoint.h"
#include "dependency/DependencyLoaderInterface.h"

namespace UKControllerPlugin::Mapping {

    auto
    MakeMappingElementManager(MappingModuleFactory& module, Dependency::DependencyLoaderInterface& dependencyLoader)
        -> std::unique_ptr<MappingElementManager>
    {
        auto mapper = std::make_unique<MappingElementManager>();
        const auto& dependency =
            dependencyLoader.LoadDependency("DEPENDENCY_MAPPING_ELEMENTS", nlohmann::json::array());
        if (!dependency.is_array()) {
            LogError("Mapping elements dependency is not an array");
            return mapper;
        }

        for (const auto& element : dependency) {
            if (!ElementValid(element)) {
                LogError("Invalid mapping element " + element.dump());
                continue;
            }

            EuroScopePlugIn::CPosition position;
            position.m_Latitude = element.at("latitude").get<double>();
            position.m_Longitude = element.at("longitude").get<double>();
            mapper->Add(std::make_shared<MappingElement>(
                element.at("id").get<int>(),
                element.at("type").get<std::string>(),
                element.at("name").get<std::string>(),
                std::set<std::shared_ptr<DisplayRule>>{},
                module.VrpDrawer(element.at("name").get<std::string>(), position)));
        }

        LogInfo("Loaded " + std::to_string(mapper->Count()) + " mapping elements");
        return mapper;
    }

    auto ElementValid(const nlohmann::json& data) -> bool
    {
        return data.is_object() && data.contains("id") && data.at("id").is_number_integer() && data.contains("type") &&
               data.at("type").is_string() && data.at("type").get<std::string>() == "visual_reference_point" &&
               data.contains("latitude") && data.at("latitude").is_number() && data.contains("longitude") &&
               data.at("longitude").is_number() && data.contains("name") && data.at("name").is_string();
    }
} // namespace UKControllerPlugin::Mapping
