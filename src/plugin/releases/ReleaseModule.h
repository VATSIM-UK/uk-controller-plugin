#pragma once

namespace UKControllerPlugin {
    namespace RadarScreen {
        class RadarRenderableCollection;
        class ConfigurableDisplayCollection;
    } // namespace RadarScreen
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
        class AsrEventHandlerCollection;
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
            Dependency::DependencyLoaderInterface& dependencies
        );

        void BootstrapRadarScreen(
            const Bootstrap::PersistenceContainer& container,
            RadarScreen::RadarRenderableCollection& renderables,
            RadarScreen::ConfigurableDisplayCollection& configurables,
            Euroscope::AsrEventHandlerCollection& asrHandlers
        );
        
        [[nodiscard]] auto GetReleaseTypesDependencyKey() -> std::string;
    }  // namespace Releases
}  // namespace UKControllerPlugin
