#pragma once

// Forward declare
namespace UKControllerPlugin {
    namespace Dependency {
        class DependencyCache;
    }  // namespace Dependency
    namespace Bootstrap {
        struct PersistenceContainer;
    }  // namespace Bootstrap
}  // namespace UKControllerPlugin
// END

namespace UKControllerPlugin {
    namespace Airfield {

        /*
            Class for bootstrapping the airfield ownership event handlers.
        */
        class AirfieldOwnershipModule
        {
            public:
                static void BootstrapPlugin(
                    UKControllerPlugin::Bootstrap::PersistenceContainer & persistence,
                    const UKControllerPlugin::Dependency::DependencyCache & dependency
                );
        };
    }  // namespace Airfield
}  // namespace UKControllerPlugin
