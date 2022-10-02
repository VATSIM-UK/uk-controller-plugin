#pragma once

namespace UKControllerPlugin {

    namespace Bootstrap {
        struct PersistenceContainer;
    } // namespace Bootstrap
    namespace Dependency {
        class DependencyLoaderInterface;
    } // namespace Dependency

    namespace Sid {
        void BootstrapPlugin(
            Bootstrap::PersistenceContainer& container, Dependency::DependencyLoaderInterface& dependencyProvider);
    } // namespace Sid
} // namespace UKControllerPlugin
