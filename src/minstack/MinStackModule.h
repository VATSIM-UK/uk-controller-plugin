#pragma once

// Forward declare
namespace UKControllerPlugin {
    namespace Plugin {
        class FunctionCallEventHandler;
    }  // namespace Plugin
    namespace Metar {
        class MetarEventHandlerCollection;
    }  // namespace Metar
    namespace RadarScreen {
        class RadarRenderableCollection;
        class ConfigurableDisplayCollection;
    }  // namespace RadarScreen
    namespace TaskManager {
        class TaskRunnerInterface;
    }  // namespace TaskManager
    namespace Euroscope {
        class AsrEventHandlerCollection;
    }  // namespace Euroscope
    namespace MinStack {
        class MinStackManager;
    }  // namespace MinStack
    namespace Windows {
        struct GdiplusBrushes;
    }  // namespace Windows
    namespace Curl {
        class CurlInterface;
    }  // namespace Curl
}  // namespace UKControllerPlugin
// END


namespace UKControllerPlugin {
    namespace MinStack {

        /*
        Factory for creating history trail renderers.
        */
        class MinStackModule
        {
            public:
                static void BootstrapPlugin(
                    std::shared_ptr<MinStackManager> & msl,
                    UKControllerPlugin::Metar::MetarEventHandlerCollection & metarEvents,
                    UKControllerPlugin::TaskManager::TaskRunnerInterface & taskManager,
                    UKControllerPlugin::Curl::CurlInterface & curl
                );
                static void BootstrapRadarScreen(
                    UKControllerPlugin::Plugin::FunctionCallEventHandler & eventHandler,
                    UKControllerPlugin::MinStack::MinStackManager & minStackManager,
                    UKControllerPlugin::RadarScreen::RadarRenderableCollection & radarRender,
                    UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection & configurableDisplays,
                    const UKControllerPlugin::Windows::GdiplusBrushes & brushes,
                    UKControllerPlugin::Euroscope::AsrEventHandlerCollection & userSettingHandlers
                );

                // The URL to get METARs from.
                static const std::string metarUrl;
        };
    }  // namespace MinStack
}  // namespace UKControllerPlugin
