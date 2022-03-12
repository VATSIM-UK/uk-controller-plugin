#pragma once

namespace UKControllerPlugin {
    namespace Euroscope {
        class AsrEventHandlerCollection;
    } // namespace Euroscope
    namespace RadarScreen {
        class RadarRenderableCollection;
        class ConfigurableDisplayCollection;
        class MenuToggleableDisplayFactory;
    } // namespace RadarScreen
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Bootstrap {
    struct PersistenceContainer;

    /**
     * An interface for classes that bootstrap things to the plugin and the radar screens.
     *
     * These should not be storing any classes, they're just for the bootstrap process. For the
     * classes that may be shared between modules, look at the ModuleFactories.
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
            Euroscope::AsrEventHandlerCollection& asrHandlers,
            const RadarScreen::MenuToggleableDisplayFactory& toggleableDisplayFactory){};
    };
} // namespace UKControllerPlugin::Bootstrap
