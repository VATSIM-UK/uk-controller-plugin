#pragma once
namespace UKControllerPlugin {
    namespace Bootstrap {
        struct PersistenceContainer;
    } // namespace Bootstrap
    namespace Plugin {
        class FunctionCallEventHandler;
    } // namespace Plugin
    namespace RadarScreen {
        class RadarRenderableCollection;
        class ConfigurableDisplayCollection;
    } // namespace RadarScreen
    namespace Command {
        class CommandHandlerCollection;
    } // namespace Command
    namespace Euroscope {
        class AsrEventHandlerCollection;
        class EuroscopePluginLoopbackInterface;
    } // namespace Euroscope
    namespace HistoryTrail {
        class HistoryTrailRenderer;
        class HistoryTrailRepository;
    } // namespace HistoryTrail
    namespace Dialog {
        class DialogManager;
    } // namespace Dialog
} // namespace UKControllerPlugin

// END

namespace UKControllerPlugin {
    namespace HistoryTrail {

        /*
            Class for bootstrapping most of the history trail module (except the renderer -
            which must be done individually for each RadarScreen instance).
        */
        class HistoryTrailModule
        {
            public:
            static void BootstrapPlugin(UKControllerPlugin::Bootstrap::PersistenceContainer& persistence);
            static void BootstrapRadarScreen(
                UKControllerPlugin::Plugin::FunctionCallEventHandler& eventHandler,
                const UKControllerPlugin::HistoryTrail::HistoryTrailRepository& trailRepo,
                UKControllerPlugin::RadarScreen::RadarRenderableCollection& radarRender,
                const UKControllerPlugin::Dialog::DialogManager& dialogManager,
                UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection& configurableDisplays,
                UKControllerPlugin::Euroscope::AsrEventHandlerCollection& asrHandlers,
                UKControllerPlugin::Command::CommandHandlerCollection& commandHandlers,
                UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface& plugin);
        };
    } // namespace HistoryTrail
} // namespace UKControllerPlugin
