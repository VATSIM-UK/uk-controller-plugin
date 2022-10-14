#pragma once
#include "timedevent/AbstractTimedEvent.h"

namespace UKControllerPlugin::Prenote {
    class PrenoteMessageCollection;
    class PrenoteMessageEventHandlerCollection;

    class PrenoteMessageTimeout : public UKControllerPlugin::TimedEvent::AbstractTimedEvent
    {
        public:
        explicit PrenoteMessageTimeout(
            std::shared_ptr<PrenoteMessageCollection> messages,
            const PrenoteMessageEventHandlerCollection& eventHandlers);
        void TimedEventTrigger() override;

        private:
        // Collection of prenote messages
        const std::shared_ptr<PrenoteMessageCollection> messages;

        // Handlers of prenote events
        const PrenoteMessageEventHandlerCollection& eventHandlers;

        static const int ACKNOWLEDGED_PRENOTE_TIMEOUT = 10;
    };
} // namespace UKControllerPlugin::Prenote
