#include "pch/pch.h"
#include "websocket/WebsocketEventProcessorCollection.h"
#include "mock/MockWebsocketEventProcessor.h"
#include "mock/MockWebsocketProtocolProcessor.h"
#include "websocket/WebsocketMessage.h"

using UKControllerPlugin::Websocket::WebsocketEventProcessorCollection;
using UKControllerPlugin::Websocket::WebsocketMessage;
using UKControllerPluginTest::Websocket::MockWebsocketEventProcessor;
using UKControllerPluginTest::Websocket::MockWebsocketProtocolProcessor;
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
                    this->protocolProcessor.reset(new NiceMock<MockWebsocketProtocolProcessor>);
                }

                std::shared_ptr<NiceMock<MockWebsocketEventProcessor>> eventProcessor;
                std::shared_ptr<NiceMock<MockWebsocketProtocolProcessor>> protocolProcessor;
                WebsocketEventProcessorCollection collection;
        };

        TEST_F(WebsocketEventProcessorCollectionTest, ItAddsEventProcessorsToTheirEvents)
        {
            std::set<std::string> channels = { "channel1", "channel2" };

            ON_CALL(*this->eventProcessor, GetSubscriptions)
                .WillByDefault(Return(channels));

            this->collection.AddProcessor(this->eventProcessor);
            EXPECT_EQ(1, this->collection.CountProcessorsForChannel("channel1"));
            EXPECT_EQ(1, this->collection.CountProcessorsForChannel("channel2"));
        }

        TEST_F(WebsocketEventProcessorCollectionTest, ItDoesntDuplicateEventProcessorsOnEvents)
        {
            std::set<std::string> channels = { "channel1", "channel1" };

            ON_CALL(*this->eventProcessor, GetSubscriptions)
                .WillByDefault(Return(channels));

            this->collection.AddProcessor(this->eventProcessor);
            EXPECT_EQ(1, this->collection.CountProcessorsForChannel("channel1"));
        }

        TEST_F(WebsocketEventProcessorCollectionTest, ItReturnsAllSubscriptions)
        {
            std::set<std::string> channels = { "channel1", "channel2" };

            ON_CALL(*this->eventProcessor, GetSubscriptions)
                .WillByDefault(Return(channels));

            this->collection.AddProcessor(this->eventProcessor);

            EXPECT_EQ(channels, this->collection.GetAllSubscriptions());
        }

        TEST_F(WebsocketEventProcessorCollectionTest, ItSendsMessagesToRegisteredProcessors)
        {
            const WebsocketMessage message = { "event1", "channel1" };
            std::set<std::string> channels = { "channel1", "channel2" };

            ON_CALL(*this->eventProcessor, GetSubscriptions)
                .WillByDefault(Return(channels));

            EXPECT_CALL(*this->eventProcessor, ProcessWebsocketMessage(message))
                .Times(1);

            this->collection.AddProcessor(this->eventProcessor);
            this->collection.ProcessEvent(message);
        }

        TEST_F(WebsocketEventProcessorCollectionTest, ItDoesntSendMessagesToNonConcernedEventProcessors)
        {
            const WebsocketMessage message = { "channel3", "event1" };
            std::set<std::string> channels = { "channel1", "channel2" };

            ON_CALL(*this->eventProcessor, GetSubscriptions)
                .WillByDefault(Return(channels));

            EXPECT_CALL(*this->eventProcessor, ProcessWebsocketMessage(_))
                .Times(0);

            this->collection.AddProcessor(this->eventProcessor);
            this->collection.ProcessEvent(message);
        }

        TEST_F(WebsocketEventProcessorCollectionTest, ItAddsProtocolProcessorsToTheirEvents)
        {
            std::set<std::string> events = { "event1", "event2" };

            ON_CALL(*this->protocolProcessor, GetEventSubscriptions)
                .WillByDefault(Return(events));

            this->collection.AddProtocolProcessor(this->protocolProcessor);
            EXPECT_EQ(1, this->collection.CountProcessorsForProtocolEvent("event1"));
            EXPECT_EQ(1, this->collection.CountProcessorsForProtocolEvent("event2"));
        }

        TEST_F(WebsocketEventProcessorCollectionTest, ItDoesntDuplicateProtocolProcessorsOnEvents)
        {
            std::set<std::string> events = { "event1", "event1" };

            ON_CALL(*this->protocolProcessor, GetEventSubscriptions)
                .WillByDefault(Return(events));

            this->collection.AddProtocolProcessor(this->protocolProcessor);
            EXPECT_EQ(1, this->collection.CountProcessorsForProtocolEvent("event1"));
        }

        TEST_F(WebsocketEventProcessorCollectionTest, ItSendsMessagesToRegisteredProtocolProcessors)
        {
            const WebsocketMessage message = { "event1", "test" };
            std::set<std::string> events = { "event1", "event2" };

            ON_CALL(*this->protocolProcessor, GetEventSubscriptions)
                .WillByDefault(Return(events));

            EXPECT_CALL(*this->protocolProcessor, ProcessProtocolMessage(message))
                .Times(1);

            this->collection.AddProtocolProcessor(this->protocolProcessor);
            this->collection.ProcessProtocolEvent(message);
        }

        TEST_F(WebsocketEventProcessorCollectionTest, ItDoesntSendMessagesToNonConcernedProtocolProcessors)
        {
            const WebsocketMessage message = { "test", "event3" };
            std::set<std::string> events = { "event1", "event2" };

            ON_CALL(*this->protocolProcessor, GetEventSubscriptions)
                .WillByDefault(Return(events));

            EXPECT_CALL(*this->protocolProcessor, ProcessProtocolMessage(_))
                .Times(0);

            this->collection.AddProtocolProcessor(this->protocolProcessor);
            this->collection.ProcessEvent(message);
        }
    }  // namespace Websocket
}  // namespace UKControllerPluginTest