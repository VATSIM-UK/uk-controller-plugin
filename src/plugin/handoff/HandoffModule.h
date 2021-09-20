#pragma once

namespace UKControllerPlugin {
    namespace Bootstrap {
        struct PersistenceContainer;
    } // namespace Bootstrap
    namespace Dependency {
        class DependencyLoaderInterface;
    } // namespace Dependency
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Handoff {

    void BootstrapPlugin(
        UKControllerPlugin::Bootstrap::PersistenceContainer& container,
        UKControllerPlugin::Dependency::DependencyLoaderInterface& dependency);

    [[nodiscard]] auto GetHandoffDependencyKey() -> std::string;
    [[nodiscard]] auto GetSidHandoffMappingDependencyKey() -> std::string;
} // namespace UKControllerPlugin::Handoff
