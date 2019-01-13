#pragma once

namespace UKControllerPlugin {
    namespace Bootstrap {
        struct PersistenceContainer;
    }  // namespace Bootstrap
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Euroscope {

        /*
        Bootstraps the user messager.
        */
        class PluginUserSettingBootstrap
        {
            public:
                static void BootstrapPlugin(
                    UKControllerPlugin::Bootstrap::PersistenceContainer & container
                );
        };
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
