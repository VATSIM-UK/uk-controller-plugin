#pragma once

namespace UKControllerPlugin {
    namespace Bootstrap {
        struct PersistenceContainer;
    } // namespace Bootstrap
    namespace Dependency {
        class DependencyLoaderInterface;
    } // namespace Dependency
    namespace RadarScreen {
        class ConfigurableDisplayCollection;
    } // namespace RadarScreen
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Stands {
    /*
        Bootstraps everything to do with stand assignment at the plugin level
    */
    void BootstrapPlugin(
        UKControllerPlugin::Bootstrap::PersistenceContainer& container,
        UKControllerPlugin::Dependency::DependencyLoaderInterface& dependencies);

    /*
        Bootstraps everything to do with stand assignment at the radar screen level
    */
    void BootstrapRadarScreen(
        const UKControllerPlugin::Bootstrap::PersistenceContainer& container,
        UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection& configurableDisplays);

    [[nodiscard]] auto GetDependencyKey() -> std::string;
} // namespace UKControllerPlugin::Stands
