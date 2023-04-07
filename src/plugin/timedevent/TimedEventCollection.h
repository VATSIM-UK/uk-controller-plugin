#pragma once

// Forward declare
namespace UKControllerPlugin {
    namespace TimedEvent {
        class AbstractTimedEvent;
    } // namespace TimedEvent
} // namespace UKControllerPlugin
// END

namespace UKControllerPlugin {
    namespace TimedEvent {

        /*
            A repository of event handlers for FlightPlan events. When an event is received, it will
            call each of the handlers in turn.
        */
        class TimedEventCollection
        {
            public:
            int CountHandlers(void) const;
            int CountHandlersForFrequency(int frequency) const;
            void Tick(int seconds) const;
            void
            RegisterEvent(std::shared_ptr<UKControllerPlugin::TimedEvent::AbstractTimedEvent> event, int frequency);

            private:
            // Vector of registered handlers
            std::map<int, std::vector<std::shared_ptr<AbstractTimedEvent>>> eventMap;
        };
    } // namespace TimedEvent
} // namespace UKControllerPlugin
