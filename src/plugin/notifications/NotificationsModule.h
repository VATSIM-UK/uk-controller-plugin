#pragma once

namespace UKControllerPlugin {
    namespace Bootstrap {
        struct PersistenceContainer;
    } // namespace Bootstrap
    namespace RadarScreen {
        class ConfigurableDisplayCollection;
    } // namespace RadarScreen
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Notifications {
    void BootstrapPlugin(Bootstrap::PersistenceContainer& container);
    void BootstrapRadarScreen(
        const Bootstrap::PersistenceContainer& container, RadarScreen::ConfigurableDisplayCollection& displays);
} // namespace UKControllerPlugin::Notifications
