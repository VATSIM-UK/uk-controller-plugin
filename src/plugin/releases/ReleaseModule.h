#pragma once

namespace UKControllerPlugin {
    namespace RadarScreen {
        class RadarRenderableCollection;
    } // namespace RadarScreen
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
    } // namespace Euroscope
    namespace Dependency {
        class DependencyLoaderInterface;
    } // namespace Dependency
    namespace Bootstrap {
        struct PersistenceContainer;
    } // namespace Bootstrap

    namespace Releases {

        void BootstrapPlugin(
            Bootstrap::PersistenceContainer& container,
            Euroscope::EuroscopePluginLoopbackInterface& plugin,
            Dependency::DependencyLoaderInterface& dependencies);

        void BootstrapRadarScreen(
            const Bootstrap::PersistenceContainer& container, RadarScreen::RadarRenderableCollection& renderables);

        [[nodiscard]] auto GetReleaseTypesDependencyKey() -> std::string;
    } // namespace Releases
} // namespace UKControllerPlugin
