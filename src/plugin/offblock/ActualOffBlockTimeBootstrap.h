#pragma once

namespace UKControllerPlugin {
    namespace Bootstrap {
        struct PersistenceContainer;
    } // namespace Bootstrap
} // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Datablock {

        /*
            Bootstraps the Actual Off Block Time TAG items.
        */
        class ActualOffBlockTimeBootstrap
        {
            public:
            static void BootstrapPlugin(const UKControllerPlugin::Bootstrap::PersistenceContainer& container);

            // The tag Item ID reserved for this item.
            static const int tagItemId = 103;
        };
    } // namespace Datablock
} // namespace UKControllerPlugin
