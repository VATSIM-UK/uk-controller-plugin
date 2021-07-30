#include "pch/stdafx.h"
#include "timedevent/TimedEventCollection.h"
#include "timedevent/AbstractTimedEvent.h"

using UKControllerPlugin::TimedEvent::AbstractTimedEvent;

namespace UKControllerPlugin {
    namespace TimedEvent {


        /*
            Returns the total number of handlers.
        */
        int TimedEventCollection::CountHandlers(void) const
        {
            int count = 0;
            for (
                std::map<int, std::vector<std::shared_ptr<AbstractTimedEvent>>>::const_iterator it =
                    this->eventMap.cbegin();
                it != this->eventMap.cend();
                ++it
            ) {
                count += it->second.size();
            }

            return count;
        }

        /*
            Returns the number of timed event handlers for a given frequency of event.
        */
        int TimedEventCollection::CountHandlersForFrequency(int frequency) const
        {
            return (this->eventMap.count(frequency) == 0) ? 0 : this->eventMap.at(frequency).size();
        }
        /*
            Called by the main plugin when Euroscope calls the "OnTimer" function.
            The parameter is the number of seconds since program startup.
        */
        void TimedEventCollection::Tick(int seconds) const
        {
            for (
                std::map<int, std::vector<std::shared_ptr<AbstractTimedEvent>>>::const_iterator it =
                    this->eventMap.cbegin();
                it != this->eventMap.cend();
                ++it
            ) {

                // If we manage to find a set of events that wans to run at this time, run them all.
                if (seconds % it->first == 0) {
                    std::vector<std::shared_ptr<AbstractTimedEvent>> events = this->eventMap.find(it->first)->second;
                    for (
                        std::vector<std::shared_ptr<AbstractTimedEvent>>::const_iterator itVector = events.cbegin();
                        itVector != events.cend();
                        ++itVector
                    ) {
                        (*itVector)->TimedEventTrigger();
                    }
                }
            }
        }

        /*
            Registers an event.
        */
        void TimedEventCollection::RegisterEvent(std::shared_ptr<AbstractTimedEvent> event, int frequency)
        {
            this->eventMap[frequency].push_back(event);
        }
    }  // namespace TimedEvent
}  // namespace UKControllerPlugin
