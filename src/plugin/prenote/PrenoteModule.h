#pragma once

namespace UKControllerPlugin {
    namespace Dependency {
        class DependencyLoaderInterface;
    } // namespace Dependency
    namespace Bootstrap {
        struct PersistenceContainer;
    } // namespace Bootstrap
    namespace Prenote {
        class DeparturePrenote;
        class PrenoteEventHandler;
    } // namespace Prenote
    namespace Controller {
        class ControllerPositionCollection;
    } // namespace Controller
    namespace RadarScreen {
        class ConfigurableDisplayCollection;
        class RadarRenderableCollection;
    } // namespace RadarScreen
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Prenote {

    /*
        Bootstraps the prenote module.
    */
    class PrenoteModule
    {
        public:
        static void BootstrapPlugin(
            UKControllerPlugin::Bootstrap::PersistenceContainer& persistence,
            UKControllerPlugin::Dependency::DependencyLoaderInterface& dependency);

        static void BootstrapRadarScreen(
            const Bootstrap::PersistenceContainer& persistence,
            RadarScreen::RadarRenderableCollection& radarRenderables,
            RadarScreen::ConfigurableDisplayCollection& configurables);

        [[nodiscard]] static auto GetDependencyKey() -> std::string;
    };
} // namespace UKControllerPlugin::Prenote
