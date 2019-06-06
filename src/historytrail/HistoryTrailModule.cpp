#include "pch/stdafx.h"
#include "historytrail/HistoryTrailModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "historytrail/HistoryTrailRepository.h"
#include "historytrail/HistoryTrailEventHandler.h"
#include "historytrail/HistoryTrailDialog.h"
#include "euroscope/RadarTargetEventHandlerCollection.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "historytrail/HistoryTrailRenderer.h"
#include "plugin/FunctionCallEventHandler.h"
#include "radarscreen/RadarRenderableCollection.h"
#include "dialog/DialogManager.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "euroscope/AsrEventHandlerCollection.h"
#include "command/CommandHandlerCollection.h"
#include "euroscope/CallbackFunction.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"

using UKControllerPlugin::HistoryTrail::HistoryTrailRepository;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::HistoryTrail::HistoryTrailEventHandler;
using UKControllerPlugin::HistoryTrail::HistoryTrailRenderer;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::RadarScreen::RadarRenderableCollection;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::Euroscope::AsrEventHandlerCollection;
using UKControllerPlugin::Command::CommandHandlerCollection;
using UKControllerPlugin::Euroscope::CallbackFunction;
using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;

namespace UKControllerPlugin {
    namespace HistoryTrail {

        /*
            Bootstrap the History Trail module at the plugin level.
        */
        void HistoryTrailModule::BootstrapPlugin(PersistenceContainer & persistence)
        {
            persistence.historyTrails.reset(new HistoryTrailRepository);

            // Handler
            std::shared_ptr<HistoryTrailEventHandler> trailHandler(
                new HistoryTrailEventHandler(*persistence.historyTrails)
            );
            persistence.radarTargetHandler->RegisterHandler(trailHandler);
            persistence.flightplanHandler->RegisterHandler(trailHandler);

            // Dialog
            std::shared_ptr<HistoryTrailDialog> dialog = std::make_shared<HistoryTrailDialog>();
            persistence.dialogManager->AddDialog(
                {
                    IDD_HISTORY_TRAIL,
                    "History Trails",
                    reinterpret_cast<DLGPROC>(dialog->WndProc),
                    reinterpret_cast<LPARAM>(dialog.get()),
                    dialog
                }
            );
        }

        /*
            Bootstrap the History Trail module at a Radar Screen level.
        */
        void HistoryTrailModule::BootstrapRadarScreen(
            FunctionCallEventHandler & eventHandler,
            const HistoryTrailRepository & trailRepo,
            RadarRenderableCollection & radarRender,
            const DialogManager & dialogManager,
            ConfigurableDisplayCollection & configurableDisplays,
            AsrEventHandlerCollection & userSettingHandlers,
            CommandHandlerCollection & commandHandlers,
            EuroscopePluginLoopbackInterface & plugin
        ) {
            int toggleCallbackFunction = eventHandler.ReserveNextDynamicFunctionId();
            std::shared_ptr<HistoryTrailRenderer> renderer(
                new HistoryTrailRenderer(trailRepo, plugin, dialogManager, toggleCallbackFunction)
            );

            radarRender.RegisterRenderer(radarRender.ReserveRendererIdentifier(), renderer, radarRender.beforeTags);
            CallbackFunction configureCallback(
                toggleCallbackFunction,
                "History Trail Configure",
                std::bind(
                    &HistoryTrailRenderer::Configure,
                    renderer,
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3
                )
            );
            eventHandler.RegisterFunctionCall(configureCallback);
            configurableDisplays.RegisterDisplay(renderer);
            userSettingHandlers.RegisterHandler(renderer);
            commandHandlers.RegisterHandler(renderer);
        }
    }  // namespace HistoryTrail
}  // namespace UKControllerPlugin
