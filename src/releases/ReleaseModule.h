#pragma once
#include "bootstrap/PersistenceContainer.h"
#include "dependency/DependencyLoaderInterface.h"

namespace UKControllerPlugin {
    namespace RadarScreen {
        class RadarRenderableCollection;
    } // namespace RadarScreen
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
    } // namespace Euroscope

    namespace Releases {

        extern const std::string enrouteReleaseTypesDependency;

        void BootstrapPlugin(
            Bootstrap::PersistenceContainer& container,
            Euroscope::EuroscopePluginLoopbackInterface& plugin,
            UKControllerPlugin::Dependency::DependencyLoaderInterface& dependencies
        );

        void BootstrapRadarScreen(
            const Bootstrap::PersistenceContainer& container,
            RadarScreen::RadarRenderableCollection& renderables
        );
    }  // namespace Releases
}  // namespace UKControllerPlugin
