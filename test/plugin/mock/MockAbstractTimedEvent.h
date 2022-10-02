#pragma once
#include "timedevent/AbstractTimedEvent.h"

namespace UKControllerPluginTest {
    namespace EventHandler {

        class MockAbstractTimedEvent : public UKControllerPlugin::TimedEvent::AbstractTimedEvent
        {
            public:
            MOCK_METHOD0(TimedEventTrigger, void(void));
        };
    } // namespace EventHandler
} // namespace UKControllerPluginTest
