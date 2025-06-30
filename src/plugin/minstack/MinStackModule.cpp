#include "MinStackConfigurationDialog.h"
#include "MinStackModule.h"
#include "MinStackRenderer.h"
#include "api/ApiException.h"
#include "euroscope/AsrEventHandlerCollection.h"
#include "euroscope/CallbackFunction.h"
#include "graphics/GdiplusBrushes.h"
#include "metar/MetarEventHandlerCollection.h"
#include "plugin/FunctionCallEventHandler.h"
#include "push/PushEventProcessorCollection.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "radarscreen/RadarRenderableCollection.h"
#include "task/TaskRunnerInterface.h"

using UKControllerPlugin::Api::ApiException;
using UKControllerPlugin::Api::ApiInterface;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::Euroscope::AsrEventHandlerCollection;
using UKControllerPlugin::Euroscope::CallbackFunction;
using UKControllerPlugin::Metar::MetarEventHandlerCollection;
using UKControllerPlugin::MinStack::MinStackManager;
using UKControllerPlugin::MinStack::MinStackRenderer;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::Push::PushEventProcessorCollection;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::RadarScreen::RadarRenderableCollection;
using UKControllerPlugin::TaskManager::TaskRunnerInterface;
using UKControllerPlugin::Windows::GdiplusBrushes;

namespace UKControllerPlugin::MinStack {

    /*
        Bootstrap the plugin part of the module.
    */
    void MinStackModule::BootstrapPlugin(
        std::shared_ptr<MinStackManager>& msl,
        TaskRunnerInterface& taskManager,
        ApiInterface& api,
        PushEventProcessorCollection& pushEventProcessors,
        DialogManager& dialogManager)
    {
        msl = std::make_shared<MinStackManager>();
        pushEventProcessors.AddProcessor(msl);

        // Create the dialog for configuration
        std::shared_ptr<MinStackConfigurationDialog> dialog = std::make_shared<MinStackConfigurationDialog>(*msl);
        dialogManager.AddDialog(
            {IDD_MINSTACK,
             "Minimum Stack Levels",
             reinterpret_cast<DLGPROC>(dialog->WndProc), // NOLINT
             reinterpret_cast<LPARAM>(dialog.get()),     // NOLINT
             dialog});

        // Get all the minstacks up front
        taskManager.QueueAsynchronousTask([&api, msl]() {
            try {
                msl->UpdateAllMsls(api.GetMinStackLevels());
                LogInfo("Loaded " + std::to_string(msl->GetAllMslKeys().size()) + " minimum stack levels");
            } catch (ApiException&) {
                LogError("ApiException when trying to get initial MSL download");
            }
        });
    }

    /*
        Bootstrap the radar screen part of the module.
    */
    void MinStackModule::BootstrapRadarScreen(
        FunctionCallEventHandler& eventHandler,
        MinStackManager& minStackManager,
        RadarRenderableCollection& radarRender,
        ConfigurableDisplayCollection& configurableDisplays,
        const GdiplusBrushes& brushes,
        AsrEventHandlerCollection& userSettingHandlers,
        const DialogManager& dialogManager)
    {
        // Create the renderer and get the ids for screen objects
        int rendererId = radarRender.ReserveRendererIdentifier();
        int configureFunctionId = eventHandler.ReserveNextDynamicFunctionId();
        std::shared_ptr<MinStackRenderer> renderer(new MinStackRenderer(
            minStackManager,
            radarRender.ReserveScreenObjectIdentifier(rendererId),
            radarRender.ReserveScreenObjectIdentifier(rendererId),
            radarRender.ReserveScreenObjectIdentifier(rendererId),
            configureFunctionId,
            brushes,
            dialogManager));

        // Add to the handlers.
        radarRender.RegisterRenderer(rendererId, renderer, radarRender.beforeTags);
        CallbackFunction configureCallback(
            configureFunctionId,
            "Min Stack Configure",
            [renderer](int functionId, const std::string& subject, RECT screenObjectArea) {
                renderer->Configure(functionId, subject, screenObjectArea);
            });

        eventHandler.RegisterFunctionCall(configureCallback);
        configurableDisplays.RegisterDisplay(renderer);
        userSettingHandlers.RegisterHandler(renderer);
    }
} // namespace UKControllerPlugin::MinStack
