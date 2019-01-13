#pragma once

// Forward declarations
namespace UKControllerPlugin {
    namespace Dependency {
        class DependencyCache;
    }  // namespace Dependency
    namespace EventHandler {
        class FlightPlanEventHandlerCollection;
    }  // namespace EventHandler
    namespace Bootstrap {
        struct PersistenceContainer;
    }  // namespace Bootstrap
}  // namespace UKControllerPlugin
// END

namespace UKControllerPlugin {
    namespace InitialAltitude {

        /*
            A class for bootstrapping the Initial Altitude module and getting
            it set up for use.
        */
        class InitialAltitudeModule
        {
            public:
                static void BootstrapPlugin(
                    UKControllerPlugin::Dependency::DependencyCache & dependency,
                    UKControllerPlugin::Bootstrap::PersistenceContainer & persistence
                );
        };
    }  // namespace InitialAltitude
}  // namespace UKControllerPlugin
