#pragma once

namespace UKControllerPlugin::Bootstrap {
    struct PersistenceContainer;
} // namespace UKControllerPlugin::Bootstrap

namespace UKControllerPlugin::Theme {

    /*
        Bootstraps the theming module.
    */
    class ThemeModule
    {
        public:
        static void BootstrapPlugin(Bootstrap::PersistenceContainer& container);
    };

} // namespace UKControllerPlugin::Theme
