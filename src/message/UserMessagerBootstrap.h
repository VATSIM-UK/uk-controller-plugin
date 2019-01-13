#pragma once

namespace UKControllerPlugin {
    namespace Bootstrap {
        struct PersistenceContainer;
    }  // namespace Bootstrap
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Message {

        /*
            Bootstraps the user messager.
        */
        class UserMessagerBootstrap
        {
            public:
                static void BootstrapPlugin(
                    UKControllerPlugin::Bootstrap::PersistenceContainer & container
                );
        };
    }  // namespace Message
}  // namespace UKControllerPlugin
