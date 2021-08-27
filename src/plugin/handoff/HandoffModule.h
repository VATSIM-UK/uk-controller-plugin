#pragma once

namespace UKControllerPlugin {
    namespace Bootstrap {
        struct PersistenceContainer;
    } // namespace Bootstrap
    namespace Dependency {
        struct DependencyLoaderInterface;
    } // namespace Dependency
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Handoff {

    extern const std::string handoffOrdersDependencyKey;
    extern const std::string handoffSidMappingsDependency;

    void BootstrapPlugin(
        UKControllerPlugin::Bootstrap::PersistenceContainer& container,
        UKControllerPlugin::Dependency::DependencyLoaderInterface& dependency);

    [[nodiscard]] auto GetHandoffDependencyKey() -> std::string;
    [[nodiscard]] auto GetSidHandoffMappingDependencyKey() -> std::string;
} // namespace UKControllerPlugin::Handoff
