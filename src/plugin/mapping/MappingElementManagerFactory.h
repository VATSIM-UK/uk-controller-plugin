#pragma once

namespace UKControllerPlugin::Dependency {
    class DependencyLoaderInterface;
} // namespace UKControllerPlugin::Dependency

namespace UKControllerPlugin::Mapping {
    class MappingElementManager;
    class MappingModuleFactory;

    [[nodiscard]] auto MakeMappingElementManager(MappingModuleFactory& module, Dependency::DependencyLoaderInterface&
                                                     dependencyLoader) ->
        std::unique_ptr<MappingElementManager>;
    [[nodiscard]] auto ElementValid(const nlohmann::json& data) -> bool;
} // namespace UKControllerPlugin::Mapping
