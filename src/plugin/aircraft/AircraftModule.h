#pragma once

namespace UKControllerPlugin {
    namespace Bootstrap {
        struct PersistenceContainer;
    } // namespace Bootstrap
    namespace Dependency {
        class DependencyLoaderInterface;
    } // namespace Dependency
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Aircraft {
    void BootstrapPlugin(Bootstrap::PersistenceContainer& container, Dependency::DependencyLoaderInterface& loader);
} // namespace UKControllerPlugin::Aircraft
