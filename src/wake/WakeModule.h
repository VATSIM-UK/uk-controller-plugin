#pragma once

namespace UKControllerPlugin {
    namespace Bootstrap {
        struct PersistenceContainer;
    }  // namespace Bootstrap
    namespace Dependency {
        class DependencyLoaderInterface;
    }  // namespace Dependency
}  // namespace UKControllerPlugin


namespace UKControllerPlugin {
    namespace Wake {

        void BootstrapPlugin(
            const UKControllerPlugin::Bootstrap::PersistenceContainer & container,
            UKControllerPlugin::Dependency::DependencyLoaderInterface& dependencies
        );

    }  // namespace Wake
}  // namespace UKControllerPlugin
