#pragma once
#include "squawk/ApiSquawkAllocation.h"
#include "timedevent/AbstractTimedEvent.h"

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
    }  // namespace Euroscope
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Squawk {

        /*
            Receives the API squawk allocation events
            and subsequently assigns them to flightplans
        */
        class ApiSquawkAllocationHandler : public UKControllerPlugin::TimedEvent::AbstractTimedEvent
        {
            public:
                ApiSquawkAllocationHandler(UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin);
                void AddAllocationEvent(UKControllerPlugin::Squawk::ApiSquawkAllocation event);
                int Count(void);
                // Inherited via AbstractTimedEvent
                void TimedEventTrigger(void) override;

            private:

                // A queue of squawk events to be processed
                std::set<UKControllerPlugin::Squawk::ApiSquawkAllocation> eventQueue;
                
                // A guard on the queue to allow it 
                std::mutex queueGuard;

                // The plugin instance, to allow squawks to be set and flightplans to be retrieved
                UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin;
        };
    }  // namespace Squawk
}  // namespace UKControllerPlugin
