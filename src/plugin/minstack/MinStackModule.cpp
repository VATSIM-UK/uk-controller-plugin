#include "pch/stdafx.h"
#include "minstack/MinStackModule.h"
#include "minstack/MinStackRenderer.h"
#include "plugin/FunctionCallEventHandler.h"
#include "minstack/MinStackManager.h"
#include "radarscreen/RadarRenderableCollection.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "graphics/GdiplusBrushes.h"
#include "euroscope/AsrEventHandlerCollection.h"
#include "task/TaskRunnerInterface.h"
#include "metar/MetarEventHandlerCollection.h"
#include "euroscope/CallbackFunction.h"
#include "push/PushEventProcessorCollection.h"
#include "api/ApiException.h"
#include "minstack/MinStackConfigurationDialog.h"

using UKControllerPlugin::MinStack::MinStackRenderer;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::RadarScreen::RadarRenderableCollection;
using UKControllerPlugin::Windows::GdiplusBrushes;
using UKControllerPlugin::MinStack::MinStackManager;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::Euroscope::AsrEventHandlerCollection;
using UKControllerPlugin::TaskManager::TaskRunnerInterface;
using UKControllerPlugin::Metar::MetarEventHandlerCollection;
using UKControllerPlugin::Euroscope::CallbackFunction;
using UKControllerPlugin::Push::PushEventProcessorCollection;
using UKControllerPlugin::Api::ApiInterface;
using UKControllerPlugin::Api::ApiException;
using UKControllerPlugin::Dialog::DialogManager;

namespace UKControllerPlugin {
    namespace MinStack {

        /*
            Bootstrap the plugin part of the module.
        */
        void MinStackModule::BootstrapPlugin(
            std::shared_ptr<MinStackManager> & msl,
            TaskRunnerInterface & taskManager,
            ApiInterface & api,
            PushEventProcessorCollection& pushEventProcessors,
            DialogManager & dialogManager
        ) {
            msl.reset(new MinStackManager);
            pushEventProcessors.AddProcessor(msl);

            // Create the dialog for configuration
            std::shared_ptr<MinStackConfigurationDialog> dialog = std::make_shared<MinStackConfigurationDialog>(*msl);
            dialogManager.AddDialog(
                {
                    IDD_MINSTACK,
                    "Minimum Stack Levels",
                    reinterpret_cast<DLGPROC>(dialog->WndProc),
                    reinterpret_cast<LPARAM>(dialog.get()),
                    dialog
                }
            );

            // Get all the minstacks up front
            taskManager.QueueAsynchronousTask([& api, msl]() {
                try {
                    msl->UpdateAllMsls(
                        api.GetMinStackLevels()
                    );
                    LogInfo("Loaded " + std::to_string(msl->GetAllMslKeys().size()) + " minimum stack levels");
                } catch (ApiException api) {
                    LogError("ApiException when trying to get initial MSL download");
                }

            });
        }

        /*
            Bootstrap the radar screen part of the module.
        */
        void MinStackModule::BootstrapRadarScreen(
            FunctionCallEventHandler & eventHandler,
            MinStackManager & minStackManager,
            RadarRenderableCollection & radarRender,
            ConfigurableDisplayCollection & configurableDisplays,
            const GdiplusBrushes & brushes,
            AsrEventHandlerCollection & userSettingHandlers,
            const DialogManager & dialogManager
        )
        {
            // Create the renderer and get the ids for screen objects
            int rendererId = radarRender.ReserveRendererIdentifier();
            int configureFunctionId = eventHandler.ReserveNextDynamicFunctionId();
            std::shared_ptr<MinStackRenderer> renderer(
                new MinStackRenderer(
                    minStackManager,
                    radarRender.ReserveScreenObjectIdentifier(rendererId),
                    radarRender.ReserveScreenObjectIdentifier(rendererId),
                    radarRender.ReserveScreenObjectIdentifier(rendererId),
                    configureFunctionId,
                    brushes,
                    dialogManager
                )
            );

            // Add to the handlers.
            radarRender.RegisterRenderer(rendererId, renderer, radarRender.beforeTags);
            CallbackFunction configureCallback(
                configureFunctionId,
                "Min Stack Configure",
                std::bind(
                    &MinStackRenderer::Configure,
                    renderer,
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3
                )
            );
            eventHandler.RegisterFunctionCall(configureCallback);
            configurableDisplays.RegisterDisplay(renderer);
            userSettingHandlers.RegisterHandler(renderer);
        }
    }  // namespace MinStack
}  // namespace UKControllerPlugin
