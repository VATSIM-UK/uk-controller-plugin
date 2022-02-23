#pragma once

namespace UKControllerPlugin {
    namespace Euroscope {
        class AsrEventHandlerCollection;
    }
    namespace RadarScreen {
        class RadarRenderableCollection;
        class ConfigurableDisplayCollection;
    } // namespace RadarScreen
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Bootstrap {
    struct PersistenceContainer;

    /**
     * An interface for classes that want to be part of the plugin bootstrapping process.
     */
    class BootstrapProviderInterface
    {
        public:
        virtual ~BootstrapProviderInterface() = default;
        virtual void BootstrapPlugin(PersistenceContainer& container){};
        virtual void BootstrapRadarScreen(
            const PersistenceContainer& container,
            RadarScreen::RadarRenderableCollection& radarRenderables,
            RadarScreen::ConfigurableDisplayCollection& configurables,
            Euroscope::AsrEventHandlerCollection& asrHandlers){};
    };
} // namespace UKControllerPlugin::Bootstrap
