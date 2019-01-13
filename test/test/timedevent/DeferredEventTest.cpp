#pragma once
#include "pch/pch.h"
#include "timedevent/DeferredEvent.h"
#include "mock/MockDeferredEventRunner.h"

using UKControllerPlugin::TimedEvent::DeferredEvent;
using UKControllerPluginTest::TimedEvent::MockDeferredEventRunner;

namespace UKControllerPluginTest {
    namespace TimedEvent {

        TEST(DeferredEventTest, LessThanMeansEarlierTimepoint)
        {
            int foo = 0;
            DeferredEvent earlier {
                std::chrono::system_clock::now() - std::chrono::seconds(15),
                std::make_unique<MockDeferredEventRunner>(foo),
            };

            DeferredEvent later {
                std::chrono::system_clock::now(),
                std::make_unique<MockDeferredEventRunner>(foo),
            };

            EXPECT_LT(earlier, later);
        }

    }  // namespace TimedEvent
}  // namespace UKControllerPluginTest
