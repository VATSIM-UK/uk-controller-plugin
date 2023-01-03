#include "AcknowledgeMissedApproachDialog.h"
#include "AcknowledgeMissedApproachTagFunction.h"
#include "ConfigureMissedApproaches.h"
#include "MissedApproachAcknowledgedPushEventProcessor.h"
#include "MissedApproachAudioAlert.h"
#include "MissedApproachButton.h"
#include "MissedApproachCollection.h"
#include "MissedApproachConfigurationDialog.h"
#include "MissedApproachIndicator.h"
#include "MissedApproachModule.h"
#include "MissedApproachOptions.h"
#include "MissedApproachRenderer.h"
#include "MissedApproachUserSettingHandler.h"
#include "NewMissedApproachPushEventHandler.h"
#include "RemoveExpiredMissedApproaches.h"
#include "ToggleMissedApproachButton.h"
#include "TriggerMissedApproach.h"
#include "TriggerMissedApproachMessageHandler.h"
#include "bootstrap/PersistenceContainer.h"
#include "dialog/DialogManager.h"
#include "euroscope/AsrEventHandlerCollection.h"
#include "euroscope/CallbackFunction.h"
#include "euroscope/UserSettingAwareCollection.h"
#include "integration/InboundIntegrationMessageHandler.h"
#include "integration/IntegrationPersistenceContainer.h"
#include "plugin/FunctionCallEventHandler.h"
#include "plugin/UKPlugin.h"
#include "push/PushEventProcessorCollection.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "tag/TagItemCollection.h"
#include "timedevent/TimedEventCollection.h"

using UKControllerPlugin::Euroscope::CallbackFunction;
using UKControllerPlugin::Tag::TagFunction;

namespace UKControllerPlugin::MissedApproach {

    const int REMOVE_APPROACHES_FREQUENCY = 30;
    const int TRIGGER_MISSED_APPROACH_TAG_FUNCTION_ID = 9020;
    const int ACKNOWLEDGE_MISSED_APPROACH_TAG_FUNCTION_ID = 9021;
    const int INDICATOR_TAG_ITEM_ID = 130;
    std::shared_ptr<MissedApproachCollection> collection;               // NOLINT
    std::shared_ptr<MissedApproachAudioAlert> audioAlert;               // NOLINT
    std::shared_ptr<MissedApproachOptions> options;                     // NOLINT
    std::shared_ptr<MissedApproachUserSettingHandler> optionsHandler;   // NOLINT
    std::shared_ptr<TriggerMissedApproach> triggerHandler;              // NOLINT
    std::shared_ptr<MissedApproachConfigurationDialog> dialog;          // NOLINT
    std::shared_ptr<AcknowledgeMissedApproachDialog> acknowledgeDialog; // NOLINT

    void BootstrapPlugin(const Bootstrap::PersistenceContainer& container)
    {
        // Global (not per-ASR) options
        options = std::make_shared<MissedApproachOptions>();
        optionsHandler = std::make_shared<MissedApproachUserSettingHandler>(
            options, *container.userSettingHandlers, *container.pluginUserSettingHandler);
        container.userSettingHandlers->RegisterHandler(optionsHandler);

        // The audio alerting service
        audioAlert = std::make_shared<MissedApproachAudioAlert>(
            options, *container.plugin, *container.airfieldOwnership, *container.windows);

        // The collectioh of missed approaches
        collection = std::make_shared<MissedApproachCollection>();
        container.timedHandler->RegisterEvent(
            std::make_shared<RemoveExpiredMissedApproaches>(collection), REMOVE_APPROACHES_FREQUENCY);
        container.pushEventProcessors->AddProcessor(std::make_shared<NewMissedApproachPushEventHandler>(
            collection, audioAlert, *container.integrationModuleContainer->outboundMessageHandler));

        // Trigger missed approach
        const auto trigger = std::make_shared<TriggerMissedApproach>(
            collection,
            *container.windows,
            *container.api,
            *container.airfieldOwnership,
            audioAlert,
            *container.integrationModuleContainer->outboundMessageHandler);
        triggerHandler = trigger;
        TagFunction triggerMissedApproachTagFunction(
            TRIGGER_MISSED_APPROACH_TAG_FUNCTION_ID,
            "Trigger Missed Approach",
            [trigger](
                UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& fp,
                UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& rt,
                const std::string& context,
                const POINT& mousePos) { trigger->Trigger(fp, rt); });
        container.pluginFunctionHandlers->RegisterFunctionCall(triggerMissedApproachTagFunction);

        // Dialog
        dialog = std::make_shared<MissedApproachConfigurationDialog>(optionsHandler, *container.airfields);

        container.dialogManager->AddDialog(Dialog::DialogData{
            IDD_MISSED_APPROACH,
            "Missed Approach",
            reinterpret_cast<DLGPROC>(dialog->WndProc), // NOLINT
            reinterpret_cast<LPARAM>(dialog.get()),     // NOLINT
            dialog});

        // Message handler
        auto messageHandler = std::make_shared<TriggerMissedApproachMessageHandler>(*triggerHandler, *container.plugin);
        container.integrationModuleContainer->inboundMessageHandler->AddProcessor(messageHandler);

        // Acknowledge message handler
        container.pushEventProcessors->AddProcessor(
            std::make_shared<MissedApproachAcknowledgedPushEventProcessor>(*collection, *container.userMessager));

        // Indicator tag item
        container.tagHandler.get()->RegisterTagItem(
            INDICATOR_TAG_ITEM_ID, std::make_shared<MissedApproachIndicator>(*collection));

        // Acknowledge tag function
        auto acknowledge = std::make_shared<AcknowledgeMissedApproachTagFunction>(
            *collection, *container.dialogManager, *container.api, *container.airfieldOwnership);
        TagFunction acknowledgeMissedApproachTagFunction(
            ACKNOWLEDGE_MISSED_APPROACH_TAG_FUNCTION_ID,
            "Acknowledge Missed Approach",
            [acknowledge](
                UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& fp,
                UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& rt,
                const std::string& context,
                const POINT& mousePos) { acknowledge->TriggerDialog(fp); });
        container.pluginFunctionHandlers->RegisterFunctionCall(acknowledgeMissedApproachTagFunction);

        // Acknowledge dialog
        acknowledgeDialog = std::make_shared<AcknowledgeMissedApproachDialog>(acknowledge);

        container.dialogManager->AddDialog(Dialog::DialogData{
            IDD_MISSED_APPROACH_ACKNOWLEDGE,
            "Missed Approach Acknowledge",
            reinterpret_cast<DLGPROC>(acknowledgeDialog->WndProc), // NOLINT
            reinterpret_cast<LPARAM>(acknowledgeDialog.get()),     // NOLINT
            acknowledgeDialog});
    }

    void BootstrapRadarScreen(
        const Bootstrap::PersistenceContainer& persistence,
        RadarScreen::RadarRenderableCollection& radarRenderables,
        RadarScreen::ConfigurableDisplayCollection& configurables,
        Euroscope::AsrEventHandlerCollection& asrHandlers)
    {
        // Alerts
        auto renderOptions = std::make_shared<MissedApproachRenderOptions>();
        asrHandlers.RegisterHandler(renderOptions);

        auto renderer = std::make_shared<MissedApproachRenderer>(
            collection, *persistence.airfieldOwnership, *persistence.plugin, renderOptions, options);
        radarRenderables.RegisterRenderer(
            radarRenderables.ReserveRendererIdentifier(), renderer, RadarScreen::RadarRenderableCollection::afterTags);

        // The button
        const auto buttonRendererId = radarRenderables.ReserveRendererIdentifier();
        const auto buttonScreenObject = radarRenderables.ReserveScreenObjectIdentifier(buttonRendererId);
        auto button = std::make_shared<MissedApproachButton>(
            collection, triggerHandler, *persistence.plugin, *persistence.airfieldOwnership, buttonScreenObject);

        radarRenderables.RegisterRenderer(buttonRendererId, button, RadarScreen::RadarRenderableCollection::afterLists);
        asrHandlers.RegisterHandler(button);

        // Button toggle
        const auto buttonToggleCallbackId = persistence.pluginFunctionHandlers->ReserveNextDynamicFunctionId();
        const auto buttonToggle = std::make_shared<ToggleMissedApproachButton>(button, buttonToggleCallbackId);
        configurables.RegisterDisplay(buttonToggle);
        CallbackFunction toggleButtonCallback(
            buttonToggleCallbackId,
            "Toggle Missed Approach Button",
            [buttonToggle](int id, const std::string& context, const RECT& pos) {
                buttonToggle->Configure(id, context, pos);
            });
        persistence.pluginFunctionHandlers->RegisterFunctionCall(toggleButtonCallback);

        // Configuration menu item
        const auto configurationCallbackId = persistence.pluginFunctionHandlers->ReserveNextDynamicFunctionId();
        const auto configuration = std::make_shared<ConfigureMissedApproaches>(
            renderOptions, *persistence.dialogManager, configurationCallbackId);
        CallbackFunction configurationCallback(
            configurationCallbackId,
            "Configure Missed Approaches",
            [configuration](int id, const std::string& context, const RECT& pos) {
                configuration->Configure(id, context, pos);
            });
        configurables.RegisterDisplay(configuration);
        persistence.pluginFunctionHandlers->RegisterFunctionCall(configurationCallback);
    }
} // namespace UKControllerPlugin::MissedApproach
