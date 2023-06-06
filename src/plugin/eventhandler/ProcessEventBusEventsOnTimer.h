#pragma once
#include "eventhandler/DrainableEuroscopeThreadEventSink.h"
#include "eventhandler/EuroscopeThreadEventProcessor.h"
#include "timedevent/AbstractTimedEvent.h"

namespace UKControllerPluginUtils::EventHandler {
    class DrainableEuroscopeThreadEventSink;
} // namespace UKControllerPluginUtils::EventHandler

namespace UKControllerPlugin::EventHandler {

    /**
     * Taps into Euroscopes OnTimer event and processes any events that have been queued up
     * as needing to be processed on the Euroscope thread.
     */
    class ProcessEventBusEventsOnTimer : public TimedEvent::AbstractTimedEvent
    {
        public:
        ProcessEventBusEventsOnTimer(
            std::shared_ptr<UKControllerPluginUtils::EventHandler::DrainableEuroscopeThreadEventSink> eventSink);

        void TimedEventTrigger() override;

        private:
        // The event sink that we will drain events from.
        std::shared_ptr<UKControllerPluginUtils::EventHandler::DrainableEuroscopeThreadEventSink> eventSink;
    };

} // namespace UKControllerPlugin::EventHandler
