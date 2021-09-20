#include "timedevent/TimedEventCollection.h"

using ::testing::StrictMock;
using UKControllerPlugin::TimedEvent::TimedEventCollection;
using UKControllerPluginTest::EventHandler::MockAbstractTimedEvent;

namespace UKControllerPluginTest {
    namespace EventHandler {

        TEST(TimedEventCollection, RunsAllEventsForAGivenTime)
        {
            std::shared_ptr<StrictMock<MockAbstractTimedEvent>> mockEvent1(new StrictMock<MockAbstractTimedEvent>);
            std::shared_ptr<StrictMock<MockAbstractTimedEvent>> mockEvent2(new StrictMock<MockAbstractTimedEvent>);
            std::shared_ptr<StrictMock<MockAbstractTimedEvent>> mockEvent3(new StrictMock<MockAbstractTimedEvent>);

            EXPECT_CALL(*mockEvent1, TimedEventTrigger()).Times(1);

            EXPECT_CALL(*mockEvent2, TimedEventTrigger()).Times(1);

            EXPECT_CALL(*mockEvent3, TimedEventTrigger()).Times(1);

            TimedEventCollection collection;
            collection.RegisterEvent(mockEvent1, 10);
            collection.RegisterEvent(mockEvent2, 10);
            collection.RegisterEvent(mockEvent3, 10);

            collection.Tick(10);
        }

        TEST(TimedEventCollection, DoesntRunEventsForDifferentTimes)
        {
            std::shared_ptr<StrictMock<MockAbstractTimedEvent>> mockEvent1(new StrictMock<MockAbstractTimedEvent>);
            std::shared_ptr<StrictMock<MockAbstractTimedEvent>> mockEvent2(new StrictMock<MockAbstractTimedEvent>);
            std::shared_ptr<StrictMock<MockAbstractTimedEvent>> mockEvent3(new StrictMock<MockAbstractTimedEvent>);

            EXPECT_CALL(*mockEvent1, TimedEventTrigger()).Times(1);

            EXPECT_CALL(*mockEvent2, TimedEventTrigger()).Times(1);

            TimedEventCollection collection;
            collection.RegisterEvent(mockEvent1, 10);
            collection.RegisterEvent(mockEvent2, 20);
            collection.RegisterEvent(mockEvent3, 15);

            collection.Tick(40);
        }

        TEST(TimedEventCollection, StartsEmpty)
        {
            TimedEventCollection collection;
            EXPECT_EQ(0, collection.CountHandlers());
        }

        TEST(TimedEventCollection, CountHandlersCountsInAllFrequencies)
        {
            TimedEventCollection collection;
            collection.RegisterEvent(std::make_shared<MockAbstractTimedEvent>(), 10);
            collection.RegisterEvent(std::make_shared<MockAbstractTimedEvent>(), 10);
            collection.RegisterEvent(std::make_shared<MockAbstractTimedEvent>(), 20);
            collection.RegisterEvent(std::make_shared<MockAbstractTimedEvent>(), 30);
            collection.RegisterEvent(std::make_shared<MockAbstractTimedEvent>(), 30);
            collection.RegisterEvent(std::make_shared<MockAbstractTimedEvent>(), 30);
            EXPECT_EQ(6, collection.CountHandlers());
        }

        TEST(TimedEventCollection, CountHandlersForFrequencyOnlyCountsInFrequency)
        {
            TimedEventCollection collection;
            collection.RegisterEvent(std::make_shared<MockAbstractTimedEvent>(), 10);
            collection.RegisterEvent(std::make_shared<MockAbstractTimedEvent>(), 10);
            collection.RegisterEvent(std::make_shared<MockAbstractTimedEvent>(), 20);
            collection.RegisterEvent(std::make_shared<MockAbstractTimedEvent>(), 30);
            collection.RegisterEvent(std::make_shared<MockAbstractTimedEvent>(), 30);
            collection.RegisterEvent(std::make_shared<MockAbstractTimedEvent>(), 30);
            EXPECT_EQ(2, collection.CountHandlersForFrequency(10));
        }

        TEST(TimedEventCollection, CountHandlersForFrequencyReturnsZeroForEmptyFrequency)
        {
            TimedEventCollection collection;
            collection.RegisterEvent(std::make_shared<MockAbstractTimedEvent>(), 10);
            collection.RegisterEvent(std::make_shared<MockAbstractTimedEvent>(), 10);
            collection.RegisterEvent(std::make_shared<MockAbstractTimedEvent>(), 20);
            collection.RegisterEvent(std::make_shared<MockAbstractTimedEvent>(), 30);
            collection.RegisterEvent(std::make_shared<MockAbstractTimedEvent>(), 30);
            collection.RegisterEvent(std::make_shared<MockAbstractTimedEvent>(), 30);
            EXPECT_EQ(0, collection.CountHandlersForFrequency(11));
        }
    } // namespace EventHandler
} // namespace UKControllerPluginTest
