#pragma once

// Forward declarations
namespace UKControllerPlugin {
    namespace Dependency {
        class DependencyLoaderInterface;
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
                    UKControllerPlugin::Dependency::DependencyLoaderInterface & dependency,
                    UKControllerPlugin::Bootstrap::PersistenceContainer & persistence
                );

                // The function id for the recycle initial altitude function
                static const int recycleFunctionId = 9002;
        };
    }  // namespace InitialAltitude
}  // namespace UKControllerPlugin
