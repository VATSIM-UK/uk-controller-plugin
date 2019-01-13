#pragma once

namespace UKControllerPlugin {
    namespace Bootstrap {
        struct PersistenceContainer;
    }  // namespace Bootstrap
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Datablock {

        /*
            Bootstraps the Actual Off Block Time TAG items.
        */
        class EstimatedOffBlockTimeBootstrap
        {
            public:
                static void BootstrapPlugin(const UKControllerPlugin::Bootstrap::PersistenceContainer & container);

                // The tag Item ID reserved for this item.
                static const int tagItemId = 102;
        };
    }  // namespace Datablock
}  // namespace UKControllerPlugin
