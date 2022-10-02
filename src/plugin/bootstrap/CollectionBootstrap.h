#pragma once

// Forward declarations
namespace UKControllerPlugin {
    namespace Bootstrap {
        struct PersistenceContainer;
    } // namespace Bootstrap
    namespace Dependency {
        class DependencyLoaderInterface;
    } // namespace Dependency
} // namespace UKControllerPlugin
// END

namespace UKControllerPlugin {
    namespace Bootstrap {

        /*
            Class for bootstrapping the various collections - such as airfields, controller
            positions etc.
        */
        class CollectionBootstrap
        {
            public:
            static void BootstrapPlugin(
                UKControllerPlugin::Bootstrap::PersistenceContainer& persistence,
                UKControllerPlugin::Dependency::DependencyLoaderInterface& dependency);
        };
    } // namespace Bootstrap
} // namespace UKControllerPlugin
