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
    namespace Wake {

        // The file we get dependencies from
        extern const std::string dependencyFile;
        extern const int tagItemId;

        void BootstrapPlugin(
            const UKControllerPlugin::Bootstrap::PersistenceContainer & container,
            const UKControllerPlugin::Dependency::DependencyCache & dependencies
        );

    }  // namespace Wake
}  // namespace UKControllerPlugin
