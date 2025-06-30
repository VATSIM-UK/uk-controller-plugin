#include "CountdownModule.h"
#include "CountdownRenderer.h"
#include "CountdownTimer.h"
#include "GlobalCountdownSettingFunctions.h"
#include "TimerConfiguration.h"
#include "TimerConfigurationDialog.h"
#include "TimerConfigurationManager.h"
#include "bootstrap/PersistenceContainer.h"
#include "dialog/DialogManager.h"
#include "euroscope/AsrEventHandlerCollection.h"
#include "euroscope/CallbackFunction.h"
#include "euroscope/UserSetting.h"
#include "graphics/GdiplusBrushes.h"
#include "plugin/FunctionCallEventHandler.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "radarscreen/RadarRenderableCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Countdown::CountdownRenderer;
using UKControllerPlugin::Countdown::CountdownTimer;
using UKControllerPlugin::Countdown::GetTimerDurationDescription;
using UKControllerPlugin::Countdown::GetTimerDurationKey;
using UKControllerPlugin::Countdown::GetTimerEnabledDescription;
using UKControllerPlugin::Countdown::GetTimerEnabledKey;
using UKControllerPlugin::Euroscope::AsrEventHandlerCollection;
using UKControllerPlugin::Euroscope::CallbackFunction;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::RadarScreen::RadarRenderableCollection;
using UKControllerPlugin::Windows::GdiplusBrushes;

namespace UKControllerPlugin::Countdown {

    const unsigned int defaultTimerDuration = 60;

    /*
        Bootstraps the core parts of the module.
    */
    void CountdownModule::BootstrapPlugin(PersistenceContainer& container)
    {
        container.countdownTimer = std::make_shared<CountdownTimer>(*container.windows);

        // Create timer manager and register callback
        unsigned int functionId = container.pluginFunctionHandlers->ReserveNextDynamicFunctionId();
        auto configurationManager = std::make_shared<TimerConfigurationManager>(*container.dialogManager, functionId);
        container.timerConfigurationManager = configurationManager;

        CallbackFunction configureCallback(
            functionId, // NOLINT
            "Countdown Timer Configure",
            [configurationManager](int functionId, std::string subject, RECT screenObjectArea) {
                configurationManager->Configure(functionId, std::move(subject), screenObjectArea);
            });

        container.pluginFunctionHandlers->RegisterFunctionCall(configureCallback);
        container.userSettingHandlers->RegisterHandler(configurationManager);

        // Add five timer placeholders to the manager
        unsigned int timerId = 1;
        configurationManager->AddTimer({timerId++, false, defaultTimerDuration});
        configurationManager->AddTimer({timerId++, false, defaultTimerDuration});
        configurationManager->AddTimer({timerId++, false, defaultTimerDuration});
        configurationManager->AddTimer({timerId++, false, defaultTimerDuration});
        configurationManager->AddTimer({timerId, false, defaultTimerDuration}); // NOLINT

        // Create the configuration dialog
        std::shared_ptr<TimerConfigurationDialog> dialog = std::make_shared<TimerConfigurationDialog>(
            *container.pluginUserSettingHandler, *container.userSettingHandlers);

        container.dialogManager->AddDialog(
            {IDD_TIMER_CONFIGURATION,
             "Countdown Timer Configuration",
             reinterpret_cast<DLGPROC>(TimerConfigurationDialog::WndProc), // NOLINT
             reinterpret_cast<LPARAM>(dialog.get()),                       // NOLINT
             dialog});
    }

    /*
        Bootstraps the Radar Screen part of the module, the part that gets displayed to the screen.
    */
    void CountdownModule::BootstrapRadarScreen(
        FunctionCallEventHandler& eventHandler,
        CountdownTimer& countdown,
        const std::shared_ptr<TimerConfigurationManager>& configManager,
        RadarRenderableCollection& radarRender,
        ConfigurableDisplayCollection& configurableDisplays,
        const UKControllerPlugin::Windows::GdiplusBrushes& brushes,
        AsrEventHandlerCollection& userSettingHandlers)
    {
        // Create the renderer and get the ids for screen objects
        int rendererId = radarRender.ReserveRendererIdentifier();
        int configureFunctionId = eventHandler.ReserveNextDynamicFunctionId();
        std::shared_ptr<CountdownRenderer> renderer(new CountdownRenderer(
            countdown,
            *configManager,
            radarRender.ReserveScreenObjectIdentifier(rendererId),
            radarRender.ReserveScreenObjectIdentifier(rendererId),
            radarRender.ReserveScreenObjectIdentifier(rendererId),
            configureFunctionId,
            brushes));

        // Add to the relevant handlers
        radarRender.RegisterRenderer(rendererId, renderer, radarRender.beforeTags);
        CallbackFunction configureCallback(
            configureFunctionId,
            "Countdown Configure",
            [renderer](int functionId, std::string subject, RECT screenObjectArea) {
                renderer->Configure(functionId, std::move(subject), screenObjectArea);
            });
        eventHandler.RegisterFunctionCall(configureCallback);
        configurableDisplays.RegisterDisplay(renderer);
        userSettingHandlers.RegisterHandler(renderer);

        // Add the configuration manager to configurable displays
        configurableDisplays.RegisterDisplay(configManager);
    }

    /*
        Save the default user settings so they're in place if not already set
    */
    void CountdownModule::LoadDefaultUserSettings(UserSetting& userSetting)
    {
        // The default timer configuration
        const std::set<TimerConfiguration> defaultConfigs = {
            {1, true, 30}, {2, true, 60}, {3, true, 90}, {4, true, 120}, {5, true, 180}};

        for (auto it = defaultConfigs.cbegin(); it != defaultConfigs.cend(); ++it) {
            if (!userSetting.HasEntry(GetTimerEnabledKey(it->timerId))) {

                // Duration
                userSetting.Save(
                    GetTimerDurationKey(it->timerId), GetTimerDurationDescription(it->timerId), it->timerDuration);

                // Enabled
                userSetting.Save(
                    GetTimerEnabledKey(it->timerId), GetTimerEnabledDescription(it->timerId), it->timerEnabled);
            }
        }
    }
} // namespace UKControllerPlugin::Countdown
