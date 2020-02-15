#pragma once
#include "api/ApiInterface.h"
#include "dialog/DialogManager.h"

// Forward declare
namespace UKControllerPlugin {
    namespace Plugin {
        class FunctionCallEventHandler;
    }  // namespace Plugin
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
    namespace Regional {
        class RegionalPressureManager;
    }  // namespace MinStack
    namespace Windows {
        struct GdiplusBrushes;
    }  // namespace Windows
    namespace Curl {
        class CurlInterface;
    }  // namespace Curl
    namespace Websocket {
        class WebsocketEventProcessorCollection;
    }  // namespace Websocket
}  // namespace UKControllerPlugin
// END


namespace UKControllerPlugin {
    namespace Regional {

        /*
            Bootstrap for regional pressures
        */
        class RegionalPressureModule
        {
            public:
                static void BootstrapPlugin(
                    std::shared_ptr<UKControllerPlugin::Regional::RegionalPressureManager> & regional,
                    UKControllerPlugin::TaskManager::TaskRunnerInterface & taskManager,
                    UKControllerPlugin::Api::ApiInterface & api,
                    UKControllerPlugin::Websocket::WebsocketEventProcessorCollection & websocketProcessors,
                    UKControllerPlugin::Dialog::DialogManager & dialogManager
                );
                static void BootstrapRadarScreen(
                    UKControllerPlugin::Plugin::FunctionCallEventHandler & eventHandler,
                    UKControllerPlugin::Regional::RegionalPressureManager& regionalPressureManager,
                    UKControllerPlugin::RadarScreen::RadarRenderableCollection & radarRender,
                    UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection & configurableDisplays,
                    const UKControllerPlugin::Windows::GdiplusBrushes & brushes,
                    UKControllerPlugin::Euroscope::AsrEventHandlerCollection & userSettingHandlers,
                    const UKControllerPlugin::Dialog::DialogManager & dialogManager
                );
        };
    }  // namespace Regional
}  // namespace UKControllerPlugin
