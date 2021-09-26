#include "MissedApproachCollection.h"
#include "MissedApproachModule.h"
#include "NewMissedApproachPushEventHandler.h"
#include "RemoveExpiredMissedApproaches.h"
#include "TriggerMissedApproach.h"
#include "bootstrap/PersistenceContainer.h"
#include "plugin/FunctionCallEventHandler.h"
#include "push/PushEventProcessorCollection.h"
#include "timedevent/TimedEventCollection.h"

using UKControllerPlugin::Tag::TagFunction;

namespace UKControllerPlugin::MissedApproach {

    const int REMOVE_APPROACHES_FREQUENCY = 30;
    const int TRIGGER_MISSED_APPROACH_TAG_FUNCTION_ID = 9020;

    void BootstrapPlugin(const Bootstrap::PersistenceContainer& container)
    {
        auto collection = std::make_shared<MissedApproachCollection>();
        container.timedHandler->RegisterEvent(
            std::make_shared<RemoveExpiredMissedApproaches>(collection), REMOVE_APPROACHES_FREQUENCY);
        container.pushEventProcessors->AddProcessor(std::make_shared<NewMissedApproachPushEventHandler>(collection));

        // Trigger missed approach
        auto trigger = std::make_shared<TriggerMissedApproach>(
            collection, *container.windows, *container.api, *container.activeCallsigns);
        TagFunction triggerMissedApproachTagFunction(
            TRIGGER_MISSED_APPROACH_TAG_FUNCTION_ID,
            "Trigger Missed Approach",
            [trigger](
                UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& fp,
                UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& rt,
                const std::string& context,
                const POINT& mousePos) { trigger->Trigger(fp); });
        container.pluginFunctionHandlers->RegisterFunctionCall(triggerMissedApproachTagFunction);
    }
} // namespace UKControllerPlugin::MissedApproach
