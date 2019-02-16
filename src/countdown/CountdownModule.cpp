#include "pch/stdafx.h"
#include "countdown/CountdownModule.h"
#include "countdown/CountdownRenderer.h"
#include "plugin/FunctionCallEventHandler.h"
#include "countdown/CountdownTimer.h"
#include "radarscreen/RadarRenderableCollection.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "graphics/GdiplusBrushes.h"
#include "euroscope/AsrEventHandlerCollection.h"
#include "euroscope/CallbackFunction.h"

using UKControllerPlugin::Countdown::CountdownRenderer;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::RadarScreen::RadarRenderableCollection;
using UKControllerPlugin::Windows::GdiplusBrushes;
using UKControllerPlugin::Countdown::CountdownTimer;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::Euroscope::AsrEventHandlerCollection;
using UKControllerPlugin::Euroscope::CallbackFunction;

namespace UKControllerPlugin {
    namespace Countdown {

        /*
            Bootstraps the core parts of the module.
        */
        void CountdownModule::BootstrapPlugin(
            std::shared_ptr<CountdownTimer> & countdownTimer,
            UKControllerPlugin::Windows::WinApiInterface & windows
        )
        {
            countdownTimer.reset(new CountdownTimer(windows));
        }

        /*
            Bootstraps the Radar Screen part of the module, the part that gets displayed to the screen.
        */
        void CountdownModule::BootstrapRadarScreen(
            FunctionCallEventHandler & eventHandler,
            CountdownTimer & countdown,
            RadarRenderableCollection & radarRender,
            ConfigurableDisplayCollection & configurableDisplays,
            const UKControllerPlugin::Windows::GdiplusBrushes & brushes,
            AsrEventHandlerCollection & userSettingHandlers
        )
        {

            // Create the renderer and get the ids for screen objects
            int rendererId = radarRender.ReserveRendererIdentifier();
            int configureFunctionId = eventHandler.ReserveNextDynamicFunctionId();
            std::shared_ptr<CountdownRenderer> renderer(
                new CountdownRenderer(
                    countdown,
                    radarRender.ReserveScreenObjectIdentifier(rendererId),
                    radarRender.ReserveScreenObjectIdentifier(rendererId),
                    radarRender.ReserveScreenObjectIdentifier(rendererId),
                    configureFunctionId,
                    brushes
                )
            );

            // Add to the relevant handlers
            radarRender.RegisterRenderer(rendererId, renderer, radarRender.beforeTags);
            CallbackFunction configureCallback(
                configureFunctionId,
                "Countdown Configure",
                std::bind(&CountdownRenderer::Configure, renderer, std::placeholders::_1, std::placeholders::_2)
            );
            eventHandler.RegisterFunctionCall(configureCallback);
            configurableDisplays.RegisterDisplay(renderer);
            userSettingHandlers.RegisterHandler(renderer);
        }
    }  // namespace Countdown
}  // namespace UKControllerPlugin
