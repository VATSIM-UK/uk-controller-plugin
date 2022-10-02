#include "regional/RegionalPressureModule.h"
#include "regional/RegionalPressureRenderer.h"
#include "plugin/FunctionCallEventHandler.h"
#include "regional/RegionalPressureManager.h"
#include "radarscreen/RadarRenderableCollection.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "graphics/GdiplusBrushes.h"
#include "euroscope/AsrEventHandlerCollection.h"
#include "task/TaskRunnerInterface.h"
#include "euroscope/CallbackFunction.h"
#include "push/PushEventProcessorCollection.h"
#include "api/ApiException.h"
#include "regional/RegionalPressureConfigurationDialog.h"
#include "regional/RegionalPressureManagerFactory.h"

using UKControllerPlugin::Api::ApiException;
using UKControllerPlugin::Api::ApiInterface;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::Euroscope::AsrEventHandlerCollection;
using UKControllerPlugin::Euroscope::CallbackFunction;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::Push::PushEventProcessorCollection;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::RadarScreen::RadarRenderableCollection;
using UKControllerPlugin::TaskManager::TaskRunnerInterface;
using UKControllerPlugin::Windows::GdiplusBrushes;

namespace UKControllerPlugin {
    namespace Regional {

        /*
            Bootstrap the plugin part of the module.
        */
        void RegionalPressureModule::BootstrapPlugin(
            std::shared_ptr<RegionalPressureManager>& regional,
            TaskRunnerInterface& taskManager,
            ApiInterface& api,
            PushEventProcessorCollection& pushEventProcessors,
            DialogManager& dialogManager,
            UKControllerPlugin::Dependency::DependencyLoaderInterface& dependency)
        {
            regional = UKControllerPlugin::Regional::Create(dependency);
            pushEventProcessors.AddProcessor(regional);

            // Create the dialog for configuration
            std::shared_ptr<RegionalPressureConfigurationDialog> dialog =
                std::make_shared<RegionalPressureConfigurationDialog>(*regional);
            dialogManager.AddDialog(
                {IDD_REGIONAL_PRESSURE,
                 "Regional Pressures",
                 reinterpret_cast<DLGPROC>(dialog->WndProc),
                 reinterpret_cast<LPARAM>(dialog.get()),
                 dialog});

            // Get all the minstacks up front
            taskManager.QueueAsynchronousTask([&api, regional]() {
                try {
                    regional->UpdateAllPressures(api.GetRegionalPressures());
                    LogInfo(
                        "Loaded " + std::to_string(regional->GetAllRegionalPressureKeys().size()) +
                        " regional pressures");
                } catch (ApiException api) {
                    LogError("ApiException when trying to get initial regional pressure download");
                }
            });
        }

        /*
            Bootstrap the radar screen part of the module.
        */
        void RegionalPressureModule::BootstrapRadarScreen(
            FunctionCallEventHandler& eventHandler,
            RegionalPressureManager& regionalPressureManager,
            RadarRenderableCollection& radarRender,
            ConfigurableDisplayCollection& configurableDisplays,
            const GdiplusBrushes& brushes,
            AsrEventHandlerCollection& userSettingHandlers,
            const DialogManager& dialogManager)
        {
            // Create the renderer and get the ids for screen objects
            int rendererId = radarRender.ReserveRendererIdentifier();
            int configureFunctionId = eventHandler.ReserveNextDynamicFunctionId();
            std::shared_ptr<RegionalPressureRenderer> renderer(new RegionalPressureRenderer(
                regionalPressureManager,
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
                "Regional pressure configure",
                std::bind(
                    &RegionalPressureRenderer::Configure,
                    renderer,
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3));
            eventHandler.RegisterFunctionCall(configureCallback);
            configurableDisplays.RegisterDisplay(renderer);
            userSettingHandlers.RegisterHandler(renderer);
        }
    } // namespace Regional
} // namespace UKControllerPlugin
