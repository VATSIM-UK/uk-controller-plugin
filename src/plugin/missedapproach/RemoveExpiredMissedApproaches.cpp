#include "MissedApproach.h"
#include "MissedApproachCollection.h"
#include "RemoveExpiredMissedApproaches.h"

namespace UKControllerPlugin::MissedApproach {
    RemoveExpiredMissedApproaches::RemoveExpiredMissedApproaches(
        std::shared_ptr<MissedApproachCollection> missedApproaches)
        : missedApproaches(std::move(missedApproaches))
    {
    }

    void RemoveExpiredMissedApproaches::TimedEventTrigger()
    {
        this->missedApproaches->RemoveWhere(
            [](const std::shared_ptr<class MissedApproach>& missed) { return missed->IsExpired(); });
    }
} // namespace UKControllerPlugin::MissedApproach
