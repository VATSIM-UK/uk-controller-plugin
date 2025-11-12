#pragma once

namespace UKControllerPlugin {
    namespace Bootstrap {
        struct PersistenceContainer;
    } // namespace Bootstrap
    namespace Dependency {
        class DependencyLoaderInterface;
    } // namespace Dependency
    namespace Euroscope {
        class AsrEventHandlerCollection;
    } // namespace Euroscope
    namespace RadarScreen {
        class MenuToggleableDisplayFactory;
        class RadarRenderableCollection;
    } // namespace RadarScreen
    namespace Windows {
        struct GdiplusBrushes;
    } // namespace Windows
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Wake {

    void BootstrapPlugin(
        UKControllerPlugin::Bootstrap::PersistenceContainer& container,
        UKControllerPlugin::Dependency::DependencyLoaderInterface& dependencies);

    void BootstrapRadarScreen(
        const UKControllerPlugin::Bootstrap::PersistenceContainer& container,
        RadarScreen::RadarRenderableCollection& renderables,
        Euroscope::AsrEventHandlerCollection& asrHandlers,
        const RadarScreen::MenuToggleableDisplayFactory& toggleableDisplayFactory,
        const Windows::GdiplusBrushes& brushes);
} // namespace UKControllerPlugin::Wake
