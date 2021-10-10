#include "MissedApproachAudioAlert.h"
#include "MissedApproachCollection.h"
#include "MissedApproachModule.h"
#include "MissedApproachOptions.h"
#include "MissedApproachRenderOptions.h"
#include "MissedApproachRenderer.h"
#include "MissedApproachUserSettingHandler.h"
#include "NewMissedApproachPushEventHandler.h"
#include "RemoveExpiredMissedApproaches.h"
#include "TriggerMissedApproach.h"
#include "bootstrap/PersistenceContainer.h"
#include "euroscope/AsrEventHandlerCollection.h"
#include "euroscope/UserSettingAwareCollection.h"
#include "plugin/FunctionCallEventHandler.h"
#include "plugin/UKPlugin.h"
#include "push/PushEventProcessorCollection.h"
#include "timedevent/TimedEventCollection.h"

using UKControllerPlugin::Tag::TagFunction;

namespace UKControllerPlugin::MissedApproach {

    const int REMOVE_APPROACHES_FREQUENCY = 30;
    const int TRIGGER_MISSED_APPROACH_TAG_FUNCTION_ID = 9020;
    std::shared_ptr<MissedApproachCollection> collection;             // NOLINT
    std::shared_ptr<MissedApproachAudioAlert> audioAlert;             // NOLINT
    std::shared_ptr<MissedApproachOptions> options;                   // NOLINT
    std::shared_ptr<MissedApproachUserSettingHandler> optionsHandler; // NOLINT

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
        container.pushEventProcessors->AddProcessor(
            std::make_shared<NewMissedApproachPushEventHandler>(collection, audioAlert));

        // Trigger missed approach
        auto trigger = std::make_shared<TriggerMissedApproach>(
            collection, *container.windows, *container.api, *container.airfieldOwnership, audioAlert);
        TagFunction triggerMissedApproachTagFunction(
            TRIGGER_MISSED_APPROACH_TAG_FUNCTION_ID,
            "Trigger Missed Approach",
            [trigger](
                UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& fp,
                UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& rt,
                const std::string& context,
                const POINT& mousePos) { trigger->Trigger(fp, rt); });
        container.pluginFunctionHandlers->RegisterFunctionCall(triggerMissedApproachTagFunction);
    }

    void BootstrapRadarScreen(
        const Bootstrap::PersistenceContainer& persistence,
        RadarScreen::RadarRenderableCollection& radarRenderables,
        RadarScreen::ConfigurableDisplayCollection& configurables,
        Euroscope::AsrEventHandlerCollection& asrHandlers)
    {
        auto renderOptions = std::make_shared<MissedApproachRenderOptions>();
        auto renderer = std::make_shared<MissedApproachRenderer>(
            collection, *persistence.airfieldOwnership, *persistence.plugin, renderOptions);
        radarRenderables.RegisterRenderer(
            radarRenderables.ReserveRendererIdentifier(), renderer, RadarScreen::RadarRenderableCollection::afterTags);
        asrHandlers.RegisterHandler(renderOptions);
    }
} // namespace UKControllerPlugin::MissedApproach
