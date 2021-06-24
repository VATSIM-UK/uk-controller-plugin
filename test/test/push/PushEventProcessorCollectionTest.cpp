#include "pch/pch.h"
#include "push/PushEventProcessorCollection.h"
#include "mock/MockPushEventProcessor.h"
#include "push/PushEventSubscription.h"
#include "push/PushEvent.h"

using UKControllerPlugin::Push::PushEventProcessorCollection;
using UKControllerPlugin::Push::PushEvent;
using UKControllerPluginTest::Push::MockPushEventProcessor;
using UKControllerPlugin::Push::PushEventSubscription;
using ::testing::Test;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Ref;
using ::testing::_;

namespace UKControllerPluginTest {
    namespace PushEvent {

        class PushEventEventProcessorCollectionTest : public Test
        {
            public:

                void SetUp()
                {
                    this->eventProcessor.reset(new NiceMock<MockPushEventProcessor>);
                }

                std::shared_ptr<NiceMock<MockPushEventProcessor>> eventProcessor;

                PushEventSubscription subChannel1 = {PushEventSubscription::SUB_TYPE_CHANNEL, "channel1"};
                PushEventSubscription subChannel2 = {PushEventSubscription::SUB_TYPE_CHANNEL, "channel2"};
                PushEventSubscription subEvent1 = {PushEventSubscription::SUB_TYPE_EVENT, "event1"};
                PushEventSubscription subEvent2 = {PushEventSubscription::SUB_TYPE_EVENT, "event2"};
                PushEventSubscription subAll = {PushEventSubscription::SUB_TYPE_ALL};

                PushEventProcessorCollection collection;
        };

        TEST_F(PushEventEventProcessorCollectionTest, ItAddsEventProcessorsToChannelSubscriptions)
        {
            std::set channels = {subChannel1, subChannel2};

            ON_CALL(*this->eventProcessor, GetSubscriptions)
                .WillByDefault(Return(channels));

            this->collection.AddProcessor(this->eventProcessor);
            EXPECT_EQ(1, this->collection.CountProcessorsForChannel("channel1"));
            EXPECT_EQ(1, this->collection.CountProcessorsForChannel("channel2"));
        }

        TEST_F(PushEventEventProcessorCollectionTest, ItDoesntDuplicateEventProcessorsOnChannels)
        {
            std::set channels = {subChannel1, subChannel1};

            ON_CALL(*this->eventProcessor, GetSubscriptions)
                .WillByDefault(Return(channels));

            this->collection.AddProcessor(this->eventProcessor);
            EXPECT_EQ(1, this->collection.CountProcessorsForChannel("channel1"));
        }

        TEST_F(PushEventEventProcessorCollectionTest, ItAddsEventProcessorsToEventSubscriptions)
        {
            std::set events = {subEvent1, subEvent1};

            ON_CALL(*this->eventProcessor, GetSubscriptions)
                .WillByDefault(Return(events));

            this->collection.AddProcessor(this->eventProcessor);
            EXPECT_EQ(1, this->collection.CountProcessorsForEvent("event1"));
            EXPECT_EQ(1, this->collection.CountProcessorsForEvent("event1"));
        }

        TEST_F(PushEventEventProcessorCollectionTest, ItDoesntDuplicateEventProcessorsOnEvents)
        {
            std::set events = {subEvent1, subEvent1};

            ON_CALL(*this->eventProcessor, GetSubscriptions)
                .WillByDefault(Return(events));

            this->collection.AddProcessor(this->eventProcessor);
            EXPECT_EQ(1, this->collection.CountProcessorsForEvent("event1"));
        }

        TEST_F(PushEventEventProcessorCollectionTest, ItReturnsAllChannelSubscriptions)
        {
            std::set channels = {subChannel1, subChannel2};

            ON_CALL(*this->eventProcessor, GetSubscriptions)
                .WillByDefault(Return(channels));

            this->collection.AddProcessor(this->eventProcessor);

            std::set<std::string> expectedChannels = { "channel1", "channel2" };
            EXPECT_EQ(expectedChannels, this->collection.GetChannelSubscriptions());
        }

        TEST_F(PushEventEventProcessorCollectionTest, ItAddsEventProcessorsToAllSubscriptions)
        {
            std::set events = {subAll};

            ON_CALL(*this->eventProcessor, GetSubscriptions)
                .WillByDefault(Return(events));

            this->collection.AddProcessor(this->eventProcessor);
            EXPECT_EQ(1, this->collection.CountProcessorsForAll());
        }

        TEST_F(PushEventEventProcessorCollectionTest, ItSendsChannelMessagesToRegisteredProcessors)
        {
            const UKControllerPlugin::Push::PushEvent message = {"event1", "channel1"};
            std::set channels = {subChannel1, subChannel2};

            ON_CALL(*this->eventProcessor, GetSubscriptions)
                .WillByDefault(Return(channels));

            EXPECT_CALL(*this->eventProcessor, ProcessWebsocketMessage(message))
                .Times(1);

            this->collection.AddProcessor(this->eventProcessor);
            this->collection.ProcessEvent(message);
        }

        TEST_F(PushEventEventProcessorCollectionTest, ItSendsChannelMessagesToAllSubscriptions)
        {
            const UKControllerPlugin::Push::PushEvent message = {"event1", "channel1"};
            std::set channels = {subAll};

            ON_CALL(*this->eventProcessor, GetSubscriptions)
                .WillByDefault(Return(channels));

            EXPECT_CALL(*this->eventProcessor, ProcessWebsocketMessage(message))
                .Times(1);

            this->collection.AddProcessor(this->eventProcessor);
            this->collection.ProcessEvent(message);
        }

        TEST_F(PushEventEventProcessorCollectionTest, ItDoesntSendChannelMessagesToNonConcernedProcessors)
        {
            const UKControllerPlugin::Push::PushEvent message = {"channel3", "event1"};
            std::set channels = {subChannel1, subChannel2};

            ON_CALL(*this->eventProcessor, GetSubscriptions)
                .WillByDefault(Return(channels));

            EXPECT_CALL(*this->eventProcessor, ProcessWebsocketMessage(_))
                .Times(0);

            this->collection.AddProcessor(this->eventProcessor);
            this->collection.ProcessEvent(message);
        }

        TEST_F(PushEventEventProcessorCollectionTest, ItSendsEventMessagesToRegisteredProcessors)
        {
            const UKControllerPlugin::Push::PushEvent message = {"event1", "channel1"};
            std::set events = {subEvent1, subEvent2};

            ON_CALL(*this->eventProcessor, GetSubscriptions)
                .WillByDefault(Return(events));

            EXPECT_CALL(*this->eventProcessor, ProcessWebsocketMessage(message))
                .Times(1);

            this->collection.AddProcessor(this->eventProcessor);
            this->collection.ProcessEvent(message);
        }

        TEST_F(PushEventEventProcessorCollectionTest, ItSendsEventMessagesToAllProcessors)
        {
            const UKControllerPlugin::Push::PushEvent message = {"event1", "channel1"};
            std::set events = {subAll};

            ON_CALL(*this->eventProcessor, GetSubscriptions)
                .WillByDefault(Return(events));

            EXPECT_CALL(*this->eventProcessor, ProcessWebsocketMessage(message))
                .Times(1);

            this->collection.AddProcessor(this->eventProcessor);
            this->collection.ProcessEvent(message);
        }

        TEST_F(PushEventEventProcessorCollectionTest, ItDoesntSendTheMessageToSameProcessorTwice)
        {
            const UKControllerPlugin::Push::PushEvent message = {"event1", "channel1"};
            std::set events = {subChannel1, subEvent1, subAll};

            ON_CALL(*this->eventProcessor, GetSubscriptions)
                .WillByDefault(Return(events));

            EXPECT_CALL(*this->eventProcessor, ProcessWebsocketMessage(message))
                .Times(1);

            this->collection.AddProcessor(this->eventProcessor);
            this->collection.ProcessEvent(message);
        }
    } // namespace PushEvent
}  // namespace UKControllerPluginTest
