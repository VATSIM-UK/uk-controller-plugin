#pragma once
#include "timedevent/AbstractTimedEvent.h"

namespace ECFMP::Plugin {
    class Sdk;
} // namespace ECFMP::Plugin

namespace UKControllerPlugin::ECFMP {

    class TriggerECFMPEventLoop : public TimedEvent::AbstractTimedEvent
    {
        public:
        TriggerECFMPEventLoop(std::shared_ptr<::ECFMP::Plugin::Sdk> sdk);
        void TimedEventTrigger() override;

        private:
        std::shared_ptr<::ECFMP::Plugin::Sdk> sdk;
    };

} // namespace UKControllerPlugin::ECFMP
