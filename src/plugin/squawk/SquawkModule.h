#pragma once

namespace UKControllerPlugin::Bootstrap {
    struct PersistenceContainer;
} // namespace UKControllerPlugin::Bootstrap

namespace UKControllerPlugin::Squawk {

    /*
        Class for bootstrapping up the squawk module and getting it hooked up to everything.
    */
    class SquawkModule
    {
        public:
        static void BootstrapPlugin(
            UKControllerPlugin::Bootstrap::PersistenceContainer& container, bool automaticAssignmentDisabled);

        // How often to check for tracked aircraft that don't have squawks.
        static const int squawkAssignmentsCheckFrequency = 5;

        // How often to check for new API allocations
        static const int allocationCheckFrequency = 3;
    };
} // namespace UKControllerPlugin::Squawk
