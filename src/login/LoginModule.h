#pragma once

// Forward declarations
namespace UKControllerPlugin {
    namespace Bootstrap {
        struct PersistenceContainer;
    }  // namespace Bootstrap
}  // namespace UKControllerPlugin
// END

namespace UKControllerPlugin {
    namespace Controller {

        /*
            A class for bootstrapping the login tracker.
        */
        class LoginModule
        {
            public:
                static void BootstrapPlugin(UKControllerPlugin::Bootstrap::PersistenceContainer & persistence);
        };
    }  // namespace Controller
}  // namespace UKControllerPlugin
