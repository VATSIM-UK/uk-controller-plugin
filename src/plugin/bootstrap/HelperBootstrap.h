#pragma once

// Forward declarations
namespace UKControllerPlugin {
    namespace Bootstrap {
        struct PersistenceContainer;
    } // namespace Bootstrap
    namespace RadarScreen {
        class ConfigurableDisplayCollection;
    } // namespace RadarScreen
} // namespace UKControllerPlugin
// END

namespace UKControllerPlugin {
    namespace Bootstrap {

        /*
            A class for bootstrapping the various "helper" classes.
        */
        class HelperBootstrap
        {
            public:
            static void Bootstrap(UKControllerPlugin::Bootstrap::PersistenceContainer& persistence);
        };
    } // namespace Bootstrap
} // namespace UKControllerPlugin
