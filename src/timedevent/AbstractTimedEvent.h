#pragma once

namespace UKControllerPlugin {
    namespace TimedEvent {

        /*
            An abstract class for a "timed" event, that is, an event that repeats
            after a certain period of time.
        */
        class AbstractTimedEvent
        {
            public:
                virtual void TimedEventTrigger(void) = 0;
        };
    }  // namespace TimedEvent
}  // namespace UKControllerPlugin
