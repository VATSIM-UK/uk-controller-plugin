#pragma once
#include "pch/pch.h"
#include "timedevent/DeferredEventHandler.h"
#include "mock/MockDeferredEventRunner.h"
#include "timedevent/DeferredEventRunnerInterface.h"

using UKControllerPlugin::TimedEvent::DeferredEventHandler;
using UKControllerPluginTest::TimedEvent::MockDeferredEventRunner;
using UKControllerPlugin::TimedEvent::DeferredEventRunnerInterface;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace TimedEvent {

        class DeferredEventHandlerTest : public Test
        {
            public:
                DeferredEventHandler handler;
        };

        TEST_F(DeferredEventHandlerTest, ItStartsEmpty)
        {
            EXPECT_EQ(0, this->handler.Count());
        }

        TEST_F(DeferredEventHandlerTest, ItAddsADeferredEvent)
        {
            int timesRun = 0;
            this->handler.DeferFor(
                std::unique_ptr<MockDeferredEventRunner>(new MockDeferredEventRunner(timesRun)),
                std::chrono::seconds(10)
            );
            EXPECT_EQ(1, this->handler.Count());
        }

        TEST_F(DeferredEventHandlerTest, GetNextEventTimeReturnsMaxTimeIfNoevent)
        {
            EXPECT_EQ((std::chrono::system_clock::time_point::max)(), this->handler.NextEventTime());
        }

        TEST_F(DeferredEventHandlerTest, GetNextEventTimeReturnsTime)
        {
            int timesRun = 0;
            this->handler.DeferFor(
                std::unique_ptr<MockDeferredEventRunner>(new MockDeferredEventRunner(timesRun)),
                std::chrono::seconds(50)
            );

            std::chrono::seconds secondsFromNow = std::chrono::duration_cast<std::chrono::seconds>(
                this->handler.NextEventTime() - std::chrono::system_clock::now()
            );
            EXPECT_TRUE(secondsFromNow.count() <= 50);
            EXPECT_TRUE(secondsFromNow.count() >= 48);
        }

        TEST_F(DeferredEventHandlerTest, ItDoesntRunNonReadyEvents)
        {
            int timesRun = 0;
            this->handler.DeferFor(
                std::unique_ptr<MockDeferredEventRunner> (new MockDeferredEventRunner(timesRun)),
                std::chrono::seconds(45)
            );
            handler.TimedEventTrigger();
            EXPECT_EQ(0, timesRun);
        }

        TEST_F(DeferredEventHandlerTest, ItRunsReadyEvents)
        {
            int timesRun = 0;
            this->handler.DeferFor(
                std::unique_ptr<MockDeferredEventRunner> (new MockDeferredEventRunner(timesRun)),
                std::chrono::seconds(-60)
            );
            handler.TimedEventTrigger();
            EXPECT_EQ(1, timesRun);
        }

        TEST_F(DeferredEventHandlerTest, ItRemovesRunEventsFromQueue)
        {
            int timesRun = 0;
            this->handler.DeferFor(
                std::unique_ptr<MockDeferredEventRunner>(new MockDeferredEventRunner(timesRun)),
                std::chrono::seconds(-60)
            );
            handler.TimedEventTrigger();
            EXPECT_EQ(0, this->handler.Count());
        }

        TEST_F(DeferredEventHandlerTest, ItRunsEventsRegardlessOfInsertionOrder)
        {
            int timesRunEvent1 = 0;
            int timesRunEvent2 = 0;
            int timesRunEvent3 = 0;

            this->handler.DeferFor(
                std::unique_ptr<MockDeferredEventRunner>(new MockDeferredEventRunner(timesRunEvent1)),
                std::chrono::seconds(-60)
            );
            this->handler.DeferFor(
                std::unique_ptr<MockDeferredEventRunner>(new MockDeferredEventRunner(timesRunEvent2)),
                std::chrono::seconds(60)
            );
            this->handler.DeferFor(
                std::unique_ptr<MockDeferredEventRunner>(new MockDeferredEventRunner(timesRunEvent3)),
                std::chrono::seconds(-60)
            );
            this->handler.TimedEventTrigger();

            EXPECT_EQ(1, timesRunEvent1);
            EXPECT_EQ(0, timesRunEvent2);
            EXPECT_EQ(1, timesRunEvent3);
        }

    }  // namespace TimedEvent
}  // namespace UKControllerPluginTest
