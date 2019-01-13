#include "pch/stdafx.h"
#include "historytrail/HistoryTrailModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "historytrail/HistoryTrailRepository.h"
#include "historytrail/HistoryTrailEventHandler.h"
#include "euroscope/RadarTargetEventHandlerCollection.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "historytrail/HistoryTrailRenderer.h"
#include "plugin/FunctionCallEventHandler.h"
#include "radarscreen/RadarRenderableCollection.h"
#include "windows/WinApiInterface.h"
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
using UKControllerPlugin::Windows::WinApiInterface;
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
            std::shared_ptr<HistoryTrailEventHandler> trailHandler(
                new HistoryTrailEventHandler(*persistence.historyTrails)
            );
            persistence.radarTargetHandler->RegisterHandler(trailHandler);
            persistence.flightplanHandler->RegisterHandler(trailHandler);
        }

        /*
            Bootstrap the History Trail module at a Radar Screen level.
        */
        void HistoryTrailModule::BootstrapRadarScreen(
            FunctionCallEventHandler & eventHandler,
            const HistoryTrailRepository & trailRepo,
            RadarRenderableCollection & radarRender,
            WinApiInterface & windows,
            ConfigurableDisplayCollection & configurableDisplays,
            AsrEventHandlerCollection & userSettingHandlers,
            CommandHandlerCollection & commandHandlers,
            EuroscopePluginLoopbackInterface & plugin
        ) {
            int toggleCallbackFunction = eventHandler.ReserveNextDynamicFunctionId();
            std::shared_ptr<HistoryTrailRenderer> renderer(
                new HistoryTrailRenderer(trailRepo, plugin, windows, toggleCallbackFunction)
            );

            radarRender.RegisterRenderer(radarRender.ReserveRendererIdentifier(), renderer, radarRender.beforeTags);
            CallbackFunction configureCallback(
                toggleCallbackFunction,
                "History Trail Configure",
                std::bind(&HistoryTrailRenderer::Configure, renderer, std::placeholders::_1)
            );
            eventHandler.RegisterFunctionCall(configureCallback);
            configurableDisplays.RegisterDisplay(renderer);
            userSettingHandlers.RegisterHandler(renderer);
            commandHandlers.RegisterHandler(renderer);
        }
    }  // namespace HistoryTrail
}  // namespace UKControllerPlugin
