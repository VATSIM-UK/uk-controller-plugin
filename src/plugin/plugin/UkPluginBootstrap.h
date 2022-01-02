#pragma once

namespace UKControllerPlugin::Bootstrap {
    struct PersistenceContainer;

    /*
        Factory to create the plugin.
    */
    class UkPluginBootstrap
    {
        public:
        static void BootstrapPlugin(UKControllerPlugin::Bootstrap::PersistenceContainer& persistence);
    };
} // namespace UKControllerPlugin::Bootstrap
