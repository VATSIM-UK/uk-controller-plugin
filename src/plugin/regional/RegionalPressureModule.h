#pragma once
#include "api/ApiInterface.h"
#include "dialog/DialogManager.h"
#include "dependency/DependencyLoaderInterface.h"

// Forward declare
namespace UKControllerPlugin {
    namespace Plugin {
        class FunctionCallEventHandler;
    } // namespace Plugin
    namespace RadarScreen {
        class RadarRenderableCollection;
        class ConfigurableDisplayCollection;
    } // namespace RadarScreen
    namespace TaskManager {
        class TaskRunnerInterface;
    } // namespace TaskManager
    namespace Euroscope {
        class AsrEventHandlerCollection;
    } // namespace Euroscope
    namespace Regional {
        class RegionalPressureManager;
    } // namespace Regional
    namespace Windows {
        struct GdiplusBrushes;
    } // namespace Windows
    namespace Curl {
        class CurlInterface;
    } // namespace Curl
    namespace Push {
        class PushEventProcessorCollection;
    } // namespace Push
} // namespace UKControllerPlugin
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
                std::shared_ptr<UKControllerPlugin::Regional::RegionalPressureManager>& regional,
                UKControllerPlugin::TaskManager::TaskRunnerInterface& taskManager,
                UKControllerPlugin::Api::ApiInterface& api,
                Push::PushEventProcessorCollection& websocketProcessors,
                UKControllerPlugin::Dialog::DialogManager& dialogManager,
                UKControllerPlugin::Dependency::DependencyLoaderInterface& dependency);
            static void BootstrapRadarScreen(
                UKControllerPlugin::Plugin::FunctionCallEventHandler& eventHandler,
                UKControllerPlugin::Regional::RegionalPressureManager& regionalPressureManager,
                UKControllerPlugin::RadarScreen::RadarRenderableCollection& radarRender,
                UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection& configurableDisplays,
                const UKControllerPlugin::Windows::GdiplusBrushes& brushes,
                UKControllerPlugin::Euroscope::AsrEventHandlerCollection& userSettingHandlers,
                const UKControllerPlugin::Dialog::DialogManager& dialogManager);
        };
    } // namespace Regional
} // namespace UKControllerPlugin
