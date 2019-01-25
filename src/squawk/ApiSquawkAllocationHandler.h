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
                explicit ApiSquawkAllocationHandler(
                    UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin
                );
                void AddAllocationToQueue(UKControllerPlugin::Squawk::ApiSquawkAllocation event);
                int Count(void);
                UKControllerPlugin::Squawk::ApiSquawkAllocation First(void) const;
                // Inherited via AbstractTimedEvent
                void TimedEventTrigger(void) override;

                const UKControllerPlugin::Squawk::ApiSquawkAllocation invalid = { "______ INVALID _______", "9999" };

            private:

                // A queue of squawk events to be processed
                std::set<UKControllerPlugin::Squawk::ApiSquawkAllocation> allocationQueue;

                // A guard on the queue to allow it
                std::mutex queueGuard;

                // The plugin instance, to allow squawks to be set and flightplans to be retrieved
                UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin;
        };
    }  // namespace Squawk
}  // namespace UKControllerPlugin
