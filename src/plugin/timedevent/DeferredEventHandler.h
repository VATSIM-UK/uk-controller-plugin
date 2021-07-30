#pragma once
#include "timedevent/DeferredEventRunnerInterface.h"
#include "timedevent/DeferredEvent.h"
#include "timedevent/AbstractTimedEvent.h"

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
    }  // namespace Euroscope
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace TimedEvent {

        /*
            A class for handling deferred events.
            Broadly speaking, it stores an event until a given time
            has passed and then runs it.
        */
        class DeferredEventHandler : public UKControllerPlugin::TimedEvent::AbstractTimedEvent
        {
            public:
                inline int Count(void) const
                {
                    return this->events.size();
                }
                void DeferFor(
                    std::unique_ptr<UKControllerPlugin::TimedEvent::DeferredEventRunnerInterface> event,
                    const std::chrono::seconds seconds
                );
                std::chrono::system_clock::time_point NextEventTime(void) const;
                void TimedEventTrigger(void);

            private:
                // The events
                std::multiset<DeferredEvent> events;
        };
    }  // namespace TimedEvent
}  // namespace UKControllerPlugin
