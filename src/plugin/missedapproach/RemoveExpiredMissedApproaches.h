#pragma once
#include "timedevent/AbstractTimedEvent.h"

namespace UKControllerPlugin::MissedApproach {
    class MissedApproachCollection;

    /**
     * Removes missed approaches that have expired.
     */
    class RemoveExpiredMissedApproaches : public TimedEvent::AbstractTimedEvent
    {
        public:
        RemoveExpiredMissedApproaches(std::shared_ptr<MissedApproachCollection> missedApproaches);
        void TimedEventTrigger() override;

        private:
        // All the missed approaches
        const std::shared_ptr<MissedApproachCollection> missedApproaches;
    };
} // namespace UKControllerPlugin::MissedApproach
