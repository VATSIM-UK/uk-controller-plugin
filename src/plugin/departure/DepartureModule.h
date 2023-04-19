#pragma once

namespace UKControllerPlugin {
    namespace Bootstrap {
        struct PersistenceContainer;
    } // namespace Bootstrap
    namespace Euroscope {
        class AsrEventHandlerCollection;
    } // namespace Euroscope
    namespace RadarScreen {
        class ConfigurableDisplayCollection;
        class RadarRenderableCollection;
    } // namespace RadarScreen
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Departure {
    void BootstrapPlugin(const Bootstrap::PersistenceContainer& container);

    void BootstrapRadarScreen(
        const Bootstrap::PersistenceContainer& container,
        RadarScreen::RadarRenderableCollection& renderables,
        RadarScreen::ConfigurableDisplayCollection& configurables,
        Euroscope::AsrEventHandlerCollection& asrHandlers);
} // namespace UKControllerPlugin::Departure
