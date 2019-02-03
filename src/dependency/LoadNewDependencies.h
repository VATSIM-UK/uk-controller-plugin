#pragma once

namespace UKControllerPlugin {
    namespace Bootstrap {
        struct PersistenceContainer;
    }  // namespace Bootstrap
    namespace Dependency {
        class DependencyCache;
    }  // namespace Dependency
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Dependency {

        void LoadNewDependencies(
            const UKControllerPlugin::Bootstrap::PersistenceContainer & container,
            UKControllerPlugin::Dependency::DependencyCache * const dependencies
        );
    }  // namespace Dependency
}  // namespace UKControllerPlugin
