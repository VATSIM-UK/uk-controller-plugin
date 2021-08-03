#include "pch/pch.h"
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
#include "countdown/TimerConfigurationDialog.h"
#include "countdown/TimerConfiguration.h"
#include "euroscope/UserSetting.h"
#include "countdown/GlobalCountdownSettingFunctions.h"

using UKControllerPlugin::Countdown::CountdownRenderer;
using UKControllerPlugin::Countdown::GetTimerDurationKey;
using UKControllerPlugin::Countdown::GetTimerDurationDescription;
using UKControllerPlugin::Countdown::GetTimerEnabledKey;
using UKControllerPlugin::Countdown::GetTimerEnabledDescription;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::RadarScreen::RadarRenderableCollection;
using UKControllerPlugin::Windows::GdiplusBrushes;
using UKControllerPlugin::Countdown::CountdownTimer;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::Euroscope::AsrEventHandlerCollection;
using UKControllerPlugin::Euroscope::CallbackFunction;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Euroscope::CallbackFunction;
using UKControllerPlugin::Euroscope::UserSetting;

namespace UKControllerPlugin {
    namespace Countdown {

        // The default timer configuration
        const std::set<TimerConfiguration> CountdownModule::defaultConfigs = {
            {1, true, 30},
            {2, true, 60},
            {3, true, 90},
            {4, true, 120},
            {5, true, 180}
        };

        /*
            Bootstraps the core parts of the module.
        */
        void CountdownModule::BootstrapPlugin(PersistenceContainer & container)
        {
            container.countdownTimer = std::make_shared<CountdownTimer>(*container.windows);

            // Create timer manager and register callback
            unsigned int functionId = container.pluginFunctionHandlers->ReserveNextDynamicFunctionId();
            container.timerConfigurationManager = std::make_shared<TimerConfigurationManager>(
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
            container.userSettingHandlers->RegisterHandler(container.timerConfigurationManager);

            // Add five timer placeholders to the manager
            container.timerConfigurationManager->AddTimer({ 1, false, 60 });
            container.timerConfigurationManager->AddTimer({ 2, false, 60 });
            container.timerConfigurationManager->AddTimer({ 3, false, 60 });
            container.timerConfigurationManager->AddTimer({ 4, false, 60 });
            container.timerConfigurationManager->AddTimer({ 5, false, 60 });

            // Create the configuration dialog
            std::shared_ptr<TimerConfigurationDialog> dialog = std::make_shared<TimerConfigurationDialog>(
                *container.pluginUserSettingHandler,
                *container.userSettingHandlers
            );
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
                    *configManager,
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

        /*
            Save the default user settings so they're in place if not already set
        */
        void CountdownModule::LoadDefaultUserSettings(UserSetting & userSetting)
        {
            for (
                std::set<TimerConfiguration>::const_iterator it = CountdownModule::defaultConfigs.cbegin();
                it != CountdownModule::defaultConfigs.cend();
                ++it
            ) {
                if (!userSetting.HasEntry(GetTimerEnabledKey(it->timerId))) {

                    // Duration
                    userSetting.Save(
                        GetTimerDurationKey(it->timerId),
                        GetTimerDurationDescription(it->timerId),
                        it->timerDuration
                    );

                    // Enabled
                    userSetting.Save(
                        GetTimerEnabledKey(it->timerId),
                        GetTimerEnabledDescription(it->timerId),
                        it->timerEnabled
                    );
                }
            }
        }
    }  // namespace Countdown
}  // namespace UKControllerPlugin
