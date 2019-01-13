#pragma once

// Forward Declare
namespace UKControllerPlugin {
    namespace Bootstrap {
        struct PersistenceContainer;
    }  // namespace Bootstrap
}  // namespace UKControllerPlugin
// END

namespace UKControllerPlugin {
    namespace IntentionCode {

        /*
            Bootstraps the IntentionCode module and registers it with
            event handlers.
        */
        class IntentionCodeModule
        {
            public:
                static void BootstrapPlugin(
                    UKControllerPlugin::Bootstrap::PersistenceContainer & container
                );

                static const int tagItemId = 101;
        };
    }  // namespace IntentionCode
}  // namespace UKControllerPlugin
