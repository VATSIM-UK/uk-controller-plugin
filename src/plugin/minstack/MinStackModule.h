#pragma once
#include "api/ApiInterface.h"
#include "dialog/DialogManager.h"

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
    namespace MinStack {
        class MinStackManager;
    } // namespace MinStack
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
    namespace MinStack {

        /*
        Factory for creating history trail renderers.
        */
        class MinStackModule
        {
            public:
            static void BootstrapPlugin(
                std::shared_ptr<MinStackManager>& msl,
                UKControllerPlugin::TaskManager::TaskRunnerInterface& taskManager,
                UKControllerPlugin::Api::ApiInterface& api,
                Push::PushEventProcessorCollection& websocketProcessors,
                UKControllerPlugin::Dialog::DialogManager& dialogManager);
            static void BootstrapRadarScreen(
                UKControllerPlugin::Plugin::FunctionCallEventHandler& eventHandler,
                UKControllerPlugin::MinStack::MinStackManager& minStackManager,
                UKControllerPlugin::RadarScreen::RadarRenderableCollection& radarRender,
                UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection& configurableDisplays,
                const UKControllerPlugin::Windows::GdiplusBrushes& brushes,
                UKControllerPlugin::Euroscope::AsrEventHandlerCollection& userSettingHandlers,
                const UKControllerPlugin::Dialog::DialogManager& dialogManager);
        };
    } // namespace MinStack
} // namespace UKControllerPlugin
