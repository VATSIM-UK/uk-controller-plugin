#pragma once

namespace UKControllerPlugin {
    namespace Bootstrap {
        struct PersistenceContainer;
    }  // namespace Bootstrap
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Squawk {

        /*
            Class for bootstrapping up the squawk module and getting it hooked up to everything.
        */
        class SquawkModule
        {
            public:
                static void BootstrapPlugin(
                    UKControllerPlugin::Bootstrap::PersistenceContainer & container,
                    bool automaticAssignmentDisabled
                );

                // How often to check for tracked aircraft that don't have squawks.
                static const int trackedAircraftCheckFrequency = 5;

                // How often to check for new API allocations
                static const int allocationCheckFrequency = 3;
        };
    }  // namespace Squawk
}  // namespace UKControllerPlugin
