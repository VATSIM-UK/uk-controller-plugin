#pragma once

namespace UKControllerPlugin::TimedEvent {

    /*
        An abstract class for a "timed" event, that is, an event that repeats
        after a certain period of time on the EuroScope clock.
    */
    class AbstractTimedEvent
    {
        public:
        AbstractTimedEvent();
        virtual ~AbstractTimedEvent();
        AbstractTimedEvent(const AbstractTimedEvent&);
        AbstractTimedEvent(AbstractTimedEvent&&) noexcept;
        auto operator=(const AbstractTimedEvent&) -> AbstractTimedEvent&;
        auto operator=(AbstractTimedEvent&&) noexcept -> AbstractTimedEvent&;
        virtual void TimedEventTrigger() = 0;
    };
} // namespace UKControllerPlugin::TimedEvent
