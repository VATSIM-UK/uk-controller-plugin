#include "pch/stdafx.h"
#include "timedevent/DeferredEventHandler.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"

using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;
using UKControllerPlugin::TimedEvent::DeferredEvent;
using UKControllerPlugin::TimedEvent::DeferredEventRunnerInterface;

namespace UKControllerPlugin {
    namespace TimedEvent {

        void DeferredEventHandler::DeferFor(
            std::unique_ptr<DeferredEventRunnerInterface> event, const std::chrono::seconds seconds)
        {
            DeferredEvent deferred{std::chrono::system_clock::now() + seconds, std::move(event)};

            this->events.insert(std::move(deferred));
        }

        /*
            Gets the time of the next scheduled event
        */
        std::chrono::system_clock::time_point DeferredEventHandler::NextEventTime(void) const
        {
            if (this->events.cbegin() == this->events.cend()) {
                return (std::chrono::system_clock::time_point::max)();
            }

            return this->events.cbegin()->runAt;
        }

        /*
            Check if there are any events to be run.
        */
        void DeferredEventHandler::TimedEventTrigger(void)
        {
            // Check events
            for (std::multiset<DeferredEvent>::iterator it = this->events.begin(); it != this->events.end();) {

                // Run any events that we're ready for and remove them from the list
                if (it->runAt < std::chrono::system_clock::now()) {
                    it->runner->Run();
                    this->events.erase(it++);
                    continue;
                }

                break;
            }
        }

    } // namespace TimedEvent
} // namespace UKControllerPlugin
