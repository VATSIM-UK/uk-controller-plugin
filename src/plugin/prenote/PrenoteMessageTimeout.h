#pragma once
#include "timedevent/AbstractTimedEvent.h"

namespace UKControllerPlugin::Prenote {
    class PrenoteMessageCollection;

    class PrenoteMessageTimeout : public UKControllerPlugin::TimedEvent::AbstractTimedEvent
    {
        public:
        explicit PrenoteMessageTimeout(std::shared_ptr<PrenoteMessageCollection> messages);
        void TimedEventTrigger() override;

        private:
        // Collection of prenote messages
        const std::shared_ptr<PrenoteMessageCollection> messages;

        static const int ACKNOWLEDGED_PRENOTE_TIMEOUT = 10;
    };
} // namespace UKControllerPlugin::Prenote
