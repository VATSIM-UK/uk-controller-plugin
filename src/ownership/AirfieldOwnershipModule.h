#pragma once

// Forward declare
namespace UKControllerPlugin {
    namespace Dependency {
        class DependencyLoaderInterface;
    }  // namespace Dependency
    namespace Bootstrap {
        struct PersistenceContainer;
    }  // namespace Bootstrap
}  // namespace UKControllerPlugin
// END

namespace UKControllerPlugin {
    namespace Ownership {

        /*
            Class for bootstrapping the airfield ownership event handlers.
        */
        class AirfieldOwnershipModule
        {
            public:
                static void BootstrapPlugin(
                    UKControllerPlugin::Bootstrap::PersistenceContainer & persistence,
                    UKControllerPlugin::Dependency::DependencyLoaderInterface& dependency
                );
        };
    }  // namespace Ownership
}  // namespace UKControllerPlugin
