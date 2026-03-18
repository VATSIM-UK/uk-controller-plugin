#pragma once

namespace UKControllerPlugin {
    namespace Bootstrap {
        struct PersistenceContainer;
    } // namespace Bootstrap
    namespace Dependency {
        class DependencyLoaderInterface;
    } // namespace Dependency
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Stands {
    /*
        Bootstraps everything to do with stand assignment at the plugin level
    */
    void BootstrapPlugin(
        UKControllerPlugin::Bootstrap::PersistenceContainer& container,
        UKControllerPlugin::Dependency::DependencyLoaderInterface& dependencies);

    [[nodiscard]] auto GetDependencyKey() -> std::string;
} // namespace UKControllerPlugin::Stands
