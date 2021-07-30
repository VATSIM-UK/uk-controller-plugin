#pragma once
// Forward declare
namespace UKControllerPlugin {
    namespace Bootstrap {
        struct PersistenceContainer;
    }  // namespace Bootstrap
}  // namespace UKControllerPlugin
// END

namespace UKControllerPlugin {
    namespace Bootstrap {

        /*
            Factory to create the plugin.
        */
        class UkPluginBootstrap
        {
            public:
                static void BootstrapPlugin(UKControllerPlugin::Bootstrap::PersistenceContainer & persistence);
        };
    }  // namespace Bootstrap
}  // namespace UKControllerPlugin
