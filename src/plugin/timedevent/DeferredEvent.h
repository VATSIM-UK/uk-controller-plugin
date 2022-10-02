#pragma once
#include "timedevent/DeferredEventRunnerInterface.h"

namespace UKControllerPlugin {
    namespace TimedEvent {

        /*
        Represents a deferred event, what to run
        and when to run it.
        */
        typedef struct DeferredEvent
        {
            const std::chrono::system_clock::time_point runAt;
            std::unique_ptr<DeferredEventRunnerInterface> runner;

            bool operator<(const DeferredEvent& compare) const
            {
                return this->runAt < compare.runAt;
            }

        } DeferredEvent;

    } // namespace TimedEvent
} // namespace UKControllerPlugin
