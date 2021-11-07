#pragma once

namespace UKControllerPlugin {
    namespace Bootstrap {
        struct PersistenceContainer;
    } // namespace Bootstrap

    namespace RadarScreen {
        class ConfigurableDisplayCollection;
        class RadarRenderableCollection;
    } // namespace RadarScreen
    namespace Euroscope {
        class AsrEventHandlerCollection;
    } // namespace Euroscope
} // namespace UKControllerPlugin

namespace UKControllerPlugin::MissedApproach {
    void BootstrapPlugin(const Bootstrap::PersistenceContainer& container);
    void BootstrapRadarScreen(
        const Bootstrap::PersistenceContainer& persistence,
        RadarScreen::RadarRenderableCollection& radarRenderables,
        RadarScreen::ConfigurableDisplayCollection& configurables,
        Euroscope::AsrEventHandlerCollection& asrHandlers);
} // namespace UKControllerPlugin::MissedApproach
