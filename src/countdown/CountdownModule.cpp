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
#include "bootstrap/PersistenceContainer.h"
#include "euroscope/CallbackFunction.h"
#include "countdown/TimerConfigurationDialog.h"

using UKControllerPlugin::Countdown::CountdownRenderer;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::RadarScreen::RadarRenderableCollection;
using UKControllerPlugin::Windows::GdiplusBrushes;
using UKControllerPlugin::Countdown::CountdownTimer;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::Euroscope::AsrEventHandlerCollection;
using UKControllerPlugin::Euroscope::CallbackFunction;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Euroscope::CallbackFunction;

namespace UKControllerPlugin {
    namespace Countdown {

        /*
            Bootstraps the core parts of the module.
        */
        void CountdownModule::BootstrapPlugin(PersistenceContainer & container)
        {
            container.countdownTimer = std::make_shared<CountdownTimer>(*container.windows);

            // Create timer manager and register callback
            unsigned int functionId = container.pluginFunctionHandlers->ReserveNextDynamicFunctionId();
            container.timerConfigurationManager = std::make_shared<TimerConfigurationManager>(
                *container.pluginUserSettingHandler,
                *container.dialogManager,
                functionId
            );

            CallbackFunction configureCallback(
                functionId,
                "Countdown Timer Configure",
                std::bind(
                    &TimerConfigurationManager::Configure,
                    container.timerConfigurationManager,
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3
                )
            );

            container.pluginFunctionHandlers->RegisterFunctionCall(configureCallback);

            std::shared_ptr<TimerConfigurationDialog> dialog = std::make_shared<TimerConfigurationDialog>();

            // Create the configuration dialog
            container.dialogManager->AddDialog(
                {
                    IDD_TIMER_CONFIGURATION,
                    "Countdown Timer Configuration",
                    reinterpret_cast<DLGPROC>(TimerConfigurationDialog::WndProc),
                    reinterpret_cast<LPARAM>(dialog.get()),
                    dialog
                }
            );
        }

        /*
            Bootstraps the Radar Screen part of the module, the part that gets displayed to the screen.
        */
        void CountdownModule::BootstrapRadarScreen(
            FunctionCallEventHandler & eventHandler,
            CountdownTimer & countdown,
            const std::shared_ptr<TimerConfigurationManager> configManager,
            RadarRenderableCollection & radarRender,
            ConfigurableDisplayCollection & configurableDisplays,
            const UKControllerPlugin::Windows::GdiplusBrushes & brushes,
            AsrEventHandlerCollection & userSettingHandlers
        ) {
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
                std::bind(
                    &CountdownRenderer::Configure,
                    renderer,
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3
                )
            );
            eventHandler.RegisterFunctionCall(configureCallback);
            configurableDisplays.RegisterDisplay(renderer);
            userSettingHandlers.RegisterHandler(renderer);

            // Add the configuration manager to configurable displays
            configurableDisplays.RegisterDisplay(configManager);
        }
    }  // namespace Countdown
}  // namespace UKControllerPlugin
