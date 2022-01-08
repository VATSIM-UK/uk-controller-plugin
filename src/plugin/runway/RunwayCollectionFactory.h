#pragma once

namespace UKControllerPlugin {
    namespace Bootstrap {
        struct PersistenceContainer;
    } // namespace Bootstrap
    namespace Dependency {
        class DependencyLoaderInterface;
    } // namespace Dependency
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Runway {
    class RunwayCollection;
    
    auto BuildRunwayCollection(const nlohmann::json& dependency) -> std::unique_ptr<RunwayCollection>;
    auto RunwayValid(const nlohmann::json& runway) -> bool;
} // namespace UKControllerPlugin::Runway
