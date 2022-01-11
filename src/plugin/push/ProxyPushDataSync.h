#pragma once
#include "timedevent/AbstractTimedEvent.h"

namespace UKControllerPlugin::Push {
    class PushEventProcessorCollection;

    /*
     * When proxy connections are created, we need to make sure their data is synced (more or less)
     * with the API. This handler triggers on a timer once ES is loaded (so we know all the handlers are registered)
     * and triggers their synced event.
     */
    class ProxyPushDataSync : public TimedEvent::AbstractTimedEvent
    {
        public:
        ProxyPushDataSync(const PushEventProcessorCollection& processors);
        void TimedEventTrigger() override;

        private:
        // All the push event processors
        const PushEventProcessorCollection& processors;

        // Has the sync been done
        bool synced;
    };
} // namespace UKControllerPlugin::Push
