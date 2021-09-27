#include "MissedApproachCollection.h"
#include "MissedApproachModule.h"
#include "NewMissedApproachPushEventHandler.h"
#include "RemoveExpiredMissedApproaches.h"
#include "bootstrap/PersistenceContainer.h"
#include "push/PushEventProcessorCollection.h"
#include "timedevent/TimedEventCollection.h"

namespace UKControllerPlugin::MissedApproach {
    const int REMOVE_APPROACHES_FREQUENCY = 30;

    void BootstrapPlugin(const Bootstrap::PersistenceContainer& container)
    {
        auto collection = std::make_shared<MissedApproachCollection>();
        container.timedHandler->RegisterEvent(
            std::make_shared<RemoveExpiredMissedApproaches>(collection), REMOVE_APPROACHES_FREQUENCY);
        container.pushEventProcessors->AddProcessor(std::make_shared<NewMissedApproachPushEventHandler>(collection));
    }
} // namespace UKControllerPlugin::MissedApproach
