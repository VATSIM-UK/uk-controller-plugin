#include "MissedApproachCollection.h"
#include "MissedApproachModule.h"
#include "RemoveExpiredMissedApproaches.h"
#include "bootstrap/PersistenceContainer.h"
#include "timedevent/TimedEventCollection.h"

namespace UKControllerPlugin::MissedApproach {
    const int REMOVE_APPROACHES_FREQUENCY = 30;

    void BootstrapPlugin(const Bootstrap::PersistenceContainer& container)
    {
        auto collection = std::make_shared<MissedApproachCollection>();
        container.timedHandler->RegisterEvent(
            std::make_shared<RemoveExpiredMissedApproaches>(collection), REMOVE_APPROACHES_FREQUENCY);
    }
} // namespace UKControllerPlugin::MissedApproach
