#pragma once

// Forward declarations
namespace UKControllerPlugin {
    namespace Bootstrap {
        struct PersistenceContainer;
    }  // namespace Bootstrap
    namespace RadarScreen {
        class ConfigurableDisplayCollection;
    }  // namespace RadarScreen
}  // namespace UKControllerPlugin
// END


namespace UKControllerPlugin {
    namespace Bootstrap {

        /*
            A class for bootstrapping the various "helper" classes.
        */
        class HelperBootstrap
        {
            public:
                static void Bootstrap(UKControllerPlugin::Bootstrap::PersistenceContainer & persistence);
                static void BootstrapApiConfigurationItem(
                    const UKControllerPlugin::Bootstrap::PersistenceContainer & persistence,
                    UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection & configurableDisplays
                );
        };
    }  // namespace Bootstrap
}  // namespace UKControllerPlugin
