#pragma once

namespace UKControllerPlugin {
    namespace Bootstrap {
        struct PersistenceContainer;
    } // namespace Bootstrap
    namespace RadarScreen {
        class ConfigurableDisplayCollection;
    } // namespace RadarScreen
} // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Plugin {

        void BootstrapPluginInformationMessage(
            const UKControllerPlugin::Bootstrap::PersistenceContainer& container,
            UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection& configurables);

    } // namespace Plugin
} // namespace UKControllerPlugin
