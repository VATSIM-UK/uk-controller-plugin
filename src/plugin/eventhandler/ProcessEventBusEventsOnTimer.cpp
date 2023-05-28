#include "ProcessEventBusEventsOnTimer.h"
#include "eventhandler/DrainableEuroscopeThreadEventSink.h"

namespace UKControllerPlugin::EventHandler {
    UKControllerPlugin::EventHandler::ProcessEventBusEventsOnTimer::ProcessEventBusEventsOnTimer(
        std::shared_ptr<UKControllerPluginUtils::EventHandler::DrainableEuroscopeThreadEventSink> eventSink)
        : eventSink(eventSink)
    {
        assert(this->eventSink && "Event sink must not be null");
    }

    void ProcessEventBusEventsOnTimer::TimedEventTrigger()
    {
        this->eventSink->Drain();
    }
} // namespace UKControllerPlugin::EventHandler
