#pragma once

namespace UKControllerPlugin {
    namespace Bootstrap {
        struct PersistenceContainer;
    } // namespace Bootstrap
    namespace RadarScreen {
        class ConfigurableDisplayCollection;
    } // namespace RadarScreen
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Api {
    void BootstrapApi(Bootstrap::PersistenceContainer& container);
    void BootstrapConfigurationMenuItem(
        const Bootstrap::PersistenceContainer& container,
        RadarScreen::ConfigurableDisplayCollection& configurableDisplays);
} // namespace UKControllerPlugin::Api
