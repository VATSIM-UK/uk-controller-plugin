#include "pch/pch.h"
#include "websocket/WebsocketEventProcessorCollection.h"
#include "mock/MockWebsocketEventProcessor.h"
#include "websocket/WebsocketSubscription.h"
#include "websocket/WebsocketMessage.h"

using UKControllerPlugin::Websocket::WebsocketEventProcessorCollection;
using UKControllerPlugin::Websocket::WebsocketMessage;
using UKControllerPluginTest::Websocket::MockWebsocketEventProcessor;
using UKControllerPlugin::Websocket::WebsocketSubscription;
using ::testing::Test;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Ref;
using ::testing::_;

namespace UKControllerPluginTest {
    namespace Websocket {

        class WebsocketEventProcessorCollectionTest : public Test
        {
            public:

                void SetUp()
                {
                    this->eventProcessor.reset(new NiceMock<MockWebsocketEventProcessor>);
                }

                std::shared_ptr<NiceMock<MockWebsocketEventProcessor>> eventProcessor;

                WebsocketSubscription subChannel1 = { WebsocketSubscription::SUB_TYPE_CHANNEL, "channel1" };
                WebsocketSubscription subChannel2 = { WebsocketSubscription::SUB_TYPE_CHANNEL, "channel2" };
                WebsocketSubscription subEvent1 = { WebsocketSubscription::SUB_TYPE_EVENT, "event1" };
                WebsocketSubscription subEvent2 = { WebsocketSubscription::SUB_TYPE_EVENT, "event2" };
                WebsocketSubscription subAll = {WebsocketSubscription::SUB_TYPE_ALL};

                WebsocketEventProcessorCollection collection;
        };

        TEST_F(WebsocketEventProcessorCollectionTest, ItAddsEventProcessorsToChannelSubscriptions)
        {
            std::set<WebsocketSubscription> channels = { subChannel1, subChannel2 };

            ON_CALL(*this->eventProcessor, GetSubscriptions)
                .WillByDefault(Return(channels));

            this->collection.AddProcessor(this->eventProcessor);
            EXPECT_EQ(1, this->collection.CountProcessorsForChannel("channel1"));
            EXPECT_EQ(1, this->collection.CountProcessorsForChannel("channel2"));
        }

        TEST_F(WebsocketEventProcessorCollectionTest, ItDoesntDuplicateEventProcessorsOnChannels)
        {
            std::set<WebsocketSubscription> channels = { subChannel1, subChannel1 };

            ON_CALL(*this->eventProcessor, GetSubscriptions)
                .WillByDefault(Return(channels));

            this->collection.AddProcessor(this->eventProcessor);
            EXPECT_EQ(1, this->collection.CountProcessorsForChannel("channel1"));
        }

        TEST_F(WebsocketEventProcessorCollectionTest, ItAddsEventProcessorsToEventSubscriptions)
        {
            std::set<WebsocketSubscription> events = { subEvent1, subEvent1 };

            ON_CALL(*this->eventProcessor, GetSubscriptions)
                .WillByDefault(Return(events));

            this->collection.AddProcessor(this->eventProcessor);
            EXPECT_EQ(1, this->collection.CountProcessorsForEvent("event1"));
            EXPECT_EQ(1, this->collection.CountProcessorsForEvent("event1"));
        }

        TEST_F(WebsocketEventProcessorCollectionTest, ItDoesntDuplicateEventProcessorsOnEvents)
        {
            std::set<WebsocketSubscription> events = { subEvent1, subEvent1 };

            ON_CALL(*this->eventProcessor, GetSubscriptions)
                .WillByDefault(Return(events));

            this->collection.AddProcessor(this->eventProcessor);
            EXPECT_EQ(1, this->collection.CountProcessorsForEvent("event1"));
        }

        TEST_F(WebsocketEventProcessorCollectionTest, ItReturnsAllChannelSubscriptions)
        {
            std::set<WebsocketSubscription> channels = { subChannel1, subChannel2 };

            ON_CALL(*this->eventProcessor, GetSubscriptions)
                .WillByDefault(Return(channels));

            this->collection.AddProcessor(this->eventProcessor);

            std::set<std::string> expectedChannels = { "channel1", "channel2" };
            EXPECT_EQ(expectedChannels, this->collection.GetChannelSubscriptions());
        }

        TEST_F(WebsocketEventProcessorCollectionTest, ItAddsEventProcessorsToAllSubscriptions)
        {
            std::set<WebsocketSubscription> events = {subAll};

            ON_CALL(*this->eventProcessor, GetSubscriptions)
                .WillByDefault(Return(events));

            this->collection.AddProcessor(this->eventProcessor);
            EXPECT_EQ(1, this->collection.CountProcessorsForAll());
        }

        TEST_F(WebsocketEventProcessorCollectionTest, ItSendsChannelMessagesToRegisteredProcessors)
        {
            const WebsocketMessage message = { "event1", "channel1" };
            std::set<WebsocketSubscription> channels = { subChannel1, subChannel2 };

            ON_CALL(*this->eventProcessor, GetSubscriptions)
                .WillByDefault(Return(channels));

            EXPECT_CALL(*this->eventProcessor, ProcessWebsocketMessage(message))
                .Times(1);

            this->collection.AddProcessor(this->eventProcessor);
            this->collection.ProcessEvent(message);
        }

        TEST_F(WebsocketEventProcessorCollectionTest, ItSendsChannelMessagesToAllSubscriptions)
        {
            const WebsocketMessage message = {"event1", "channel1"};
            std::set<WebsocketSubscription> channels = {subAll};

            ON_CALL(*this->eventProcessor, GetSubscriptions)
                .WillByDefault(Return(channels));

            EXPECT_CALL(*this->eventProcessor, ProcessWebsocketMessage(message))
                .Times(1);

            this->collection.AddProcessor(this->eventProcessor);
            this->collection.ProcessEvent(message);
        }

        TEST_F(WebsocketEventProcessorCollectionTest, ItDoesntSendChannelMessagesToNonConcernedProcessors)
        {
            const WebsocketMessage message = { "channel3", "event1" };
            std::set<WebsocketSubscription> channels = { subChannel1, subChannel2 };

            ON_CALL(*this->eventProcessor, GetSubscriptions)
                .WillByDefault(Return(channels));

            EXPECT_CALL(*this->eventProcessor, ProcessWebsocketMessage(_))
                .Times(0);

            this->collection.AddProcessor(this->eventProcessor);
            this->collection.ProcessEvent(message);
        }

        TEST_F(WebsocketEventProcessorCollectionTest, ItSendsEventMessagesToRegisteredProcessors)
        {
            const WebsocketMessage message = { "event1", "channel1" };
            std::set<WebsocketSubscription> events = { subEvent1, subEvent2 };

            ON_CALL(*this->eventProcessor, GetSubscriptions)
                .WillByDefault(Return(events));

            EXPECT_CALL(*this->eventProcessor, ProcessWebsocketMessage(message))
                .Times(1);

            this->collection.AddProcessor(this->eventProcessor);
            this->collection.ProcessEvent(message);
        }

        TEST_F(WebsocketEventProcessorCollectionTest, ItSendsEventMessagesToAllProcessors)
        {
            const WebsocketMessage message = {"event1", "channel1"};
            std::set<WebsocketSubscription> events = {subAll};

            ON_CALL(*this->eventProcessor, GetSubscriptions)
                .WillByDefault(Return(events));

            EXPECT_CALL(*this->eventProcessor, ProcessWebsocketMessage(message))
                .Times(1);

            this->collection.AddProcessor(this->eventProcessor);
            this->collection.ProcessEvent(message);
        }

        TEST_F(WebsocketEventProcessorCollectionTest, ItDoesntSendTheMessageToSameProcessorTwice)
        {
            const WebsocketMessage message = { "event1", "channel1" };
            std::set<WebsocketSubscription> events = {subChannel1, subEvent1, subAll};

            ON_CALL(*this->eventProcessor, GetSubscriptions)
                .WillByDefault(Return(events));

            EXPECT_CALL(*this->eventProcessor, ProcessWebsocketMessage(message))
                .Times(1);

            this->collection.AddProcessor(this->eventProcessor);
            this->collection.ProcessEvent(message);
        }
    }  // namespace Websocket
}  // namespace UKControllerPluginTest
