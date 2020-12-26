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

        extern const int tagItemId;
        extern const int tagItemRecatId;
        extern const int tagItemStandaloneId;
        extern const std::string wakeDependencyKey;

        void BootstrapPlugin(
            const UKControllerPlugin::Bootstrap::PersistenceContainer & container,
            UKControllerPlugin::Dependency::DependencyLoaderInterface& dependencies
        );

    }  // namespace Wake
}  // namespace UKControllerPlugin
