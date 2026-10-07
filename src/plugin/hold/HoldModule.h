#pragma once

namespace UKControllerPlugin {
    namespace Bootstrap {
        struct PersistenceContainer;
    } // namespace Bootstrap
    namespace RadarScreen {
        class RadarRenderableCollection;
        class ConfigurableDisplayCollection;
    } // namespace RadarScreen
    namespace Plugin {
        class FunctionCallEventHandler;
    } // namespace Plugin
    namespace Dependency {
        class DependencyLoaderInterface;
    } // namespace Dependency
    namespace Api {
        class ApiInterface;
    } // namespace Api
    namespace Windows {
        class WinApiInterface;
    } // namespace Windows
    namespace Euroscope {
        class AsrEventHandlerCollection;
    } // namespace Euroscope
    namespace Command {
        class CommandHandlerCollection;
    } // namespace Command
    namespace Windows {
        struct GdiplusBrushes;
    } // namespace Windows
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Hold {

    extern const int selectedHoldTagItemId;
    extern const unsigned int popupMenuTagItemId;

    void BootstrapPlugin(
        UKControllerPlugin::Dependency::DependencyLoaderInterface& dependencyProvider,
        UKControllerPlugin::Bootstrap::PersistenceContainer& container);

    void BootstrapRadarScreen(
        UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection& configurableDisplay,
        UKControllerPlugin::RadarScreen::RadarRenderableCollection& radarRenderables,
        UKControllerPlugin::Euroscope::AsrEventHandlerCollection& asrEvents,
        UKControllerPlugin::Command::CommandHandlerCollection& commandHandlers,
        const UKControllerPlugin::Windows::GdiplusBrushes& brushes,
        const UKControllerPlugin::Bootstrap::PersistenceContainer& container);

    [[nodiscard]] auto GetDependencyKey() -> std::string;
} // namespace UKControllerPlugin::Hold
