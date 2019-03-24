#include "pch/stdafx.h"
#include "minstack/MinStackModule.h"
#include "minstack/MinStackRenderer.h"
#include "plugin/FunctionCallEventHandler.h"
#include "minstack/MinStackManager.h"
#include "radarscreen/RadarRenderableCollection.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "graphics/GdiplusBrushes.h"
#include "euroscope/AsrEventHandlerCollection.h"
#include "minstack/TerminalControlArea.h"
#include "task/TaskRunnerInterface.h"
#include "curl/CurlInterface.h"
#include "curl/CurlResponse.h"
#include "metar/MetarEventHandlerCollection.h"
#include "euroscope/CallbackFunction.h"
#include "curl/CurlRequest.h"

using UKControllerPlugin::MinStack::MinStackRenderer;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::RadarScreen::RadarRenderableCollection;
using UKControllerPlugin::Windows::GdiplusBrushes;
using UKControllerPlugin::MinStack::MinStackManager;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::Euroscope::AsrEventHandlerCollection;
using UKControllerPlugin::MinStack::TerminalControlArea;
using UKControllerPlugin::TaskManager::TaskRunnerInterface;
using UKControllerPlugin::Curl::CurlInterface;
using UKControllerPlugin::Curl::CurlResponse;
using UKControllerPlugin::Metar::MetarEventHandlerCollection;
using UKControllerPlugin::Euroscope::CallbackFunction;
using UKControllerPlugin::Curl::CurlRequest;

namespace UKControllerPlugin {
    namespace MinStack {

        const std::string MinStackModule::metarUrl = "http://metar.vatsim.net/metar.php?id=";

        /*
            Bootstrap the plugin part of the module.
        */
        void MinStackModule::BootstrapPlugin(
            std::shared_ptr<MinStackManager> & msl,
            MetarEventHandlerCollection & metarEvents,
            TaskRunnerInterface & taskManager,
            CurlInterface & curl
        ) {
            msl.reset(new MinStackManager);
            metarEvents.RegisterHandler(msl);
            std::vector<std::shared_ptr<TerminalControlArea>> list = {
                std::shared_ptr<TerminalControlArea>(new TerminalControlArea(L"LTMA", "LTMA", 6000, "EGLL", true)),
                std::shared_ptr<TerminalControlArea>(new TerminalControlArea(L"MTMA", "MTMA", 5000, "EGCC", true)),
                std::shared_ptr<TerminalControlArea>(new TerminalControlArea(L"STMA", "STMA", 6000, "EGPF", false)),
                std::shared_ptr<TerminalControlArea>(new TerminalControlArea(L"CICZ", "CICZ", 5000, "EGJJ", true))
            };


            // For each of the TMAs, schedule an asynchronous task to go and get the relevant METAR
            for (
                std::vector<std::shared_ptr<TerminalControlArea>>::iterator it = list.begin();
                it != list.end();
                ++it
            ) {
                std::shared_ptr<TerminalControlArea> tma = *it;
                msl->AddTerminalControlArea(tma);
                taskManager.QueueAsynchronousTask([msl, tma, &curl]() {

                    CurlRequest request(
                        MinStackModule::metarUrl + tma->GetCalculationAirfield(),
                        CurlRequest::METHOD_GET
                    );
                    CurlResponse response = curl.MakeCurlRequest(request);

                    // We had a curl error, so don't update the MSL.
                    if (response.IsCurlError() || !response.StatusOk()) {
                        return;
                    }

                    msl->NewMetar(tma->GetCalculationAirfield(), response.GetResponse());
                    LogInfo("Set MSL for " + std::string(tma->GetCharName()));
                });
            }
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
            AsrEventHandlerCollection & userSettingHandlers
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
                    brushes
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
