#include "pch/pch.h"
#include "websocket/WebsocketEventProcessorCollection.h"
#include "mock/MockWebsocketEventProcessor.h"
#include "websocket/WebsocketMessage.h"

using UKControllerPlugin::Websocket::WebsocketEventProcessorCollection;
using UKControllerPlugin::Websocket::WebsocketMessage;
using UKControllerPluginTest::Websocket::MockWebsockerEventProcessor;
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
                    this->processor.reset(new NiceMock<MockWebsockerEventProcessor>);
                }

                std::shared_ptr<NiceMock<MockWebsockerEventProcessor>> processor;
                WebsocketEventProcessorCollection collection;
        };

        TEST_F(WebsocketEventProcessorCollectionTest, ItAddsProcessorsToTheirEvents)
        {
            std::set<std::string> events = { "event1", "event2" };

            ON_CALL(*this->processor, GetSubscriptions)
                .WillByDefault(Return(events));

            this->collection.AddProcessor(processor);
            EXPECT_EQ(1, this->collection.CountProcessorsForEvent("event1"));
            EXPECT_EQ(1, this->collection.CountProcessorsForEvent("event2"));
        }

        TEST_F(WebsocketEventProcessorCollectionTest, ItDoesntDuplicateProcessorsOnEvents)
        {
            std::set<std::string> events = { "event1", "event1" };

            ON_CALL(*this->processor, GetSubscriptions)
                .WillByDefault(Return(events));

            this->collection.AddProcessor(processor);
            EXPECT_EQ(1, this->collection.CountProcessorsForEvent("event1"));
        }

        TEST_F(WebsocketEventProcessorCollectionTest, ItReturnsAllSubscriptions)
        {
            std::set<std::string> events = { "event1", "event2" };

            ON_CALL(*this->processor, GetSubscriptions)
                .WillByDefault(Return(events));

            this->collection.AddProcessor(processor);

            EXPECT_EQ(events, this->collection.GetAllSubscriptions());
        }

        TEST_F(WebsocketEventProcessorCollectionTest, ItSendsMessagesToRegisteredProcessors)
        {
            const WebsocketMessage message = {"test", "event1"};
            std::set<std::string> events = { "event1", "event2" };

            ON_CALL(*this->processor, GetSubscriptions)
                .WillByDefault(Return(events));

            EXPECT_CALL(*this->processor, ProcessWebsocketMessage(message))
                .Times(1);

            this->collection.AddProcessor(processor);
            this->collection.ProcessEvent(message);
        }

        TEST_F(WebsocketEventProcessorCollectionTest, ItDoesntSendMessagesToNonConcernedProcessors)
        {
            const WebsocketMessage message = { "test", "event3" };
            std::set<std::string> events = { "event1", "event2" };

            ON_CALL(*this->processor, GetSubscriptions)
                .WillByDefault(Return(events));

            EXPECT_CALL(*this->processor, ProcessWebsocketMessage(_))
                .Times(0);

            this->collection.AddProcessor(processor);
            this->collection.ProcessEvent(message);
        }

    }  // namespace Websocket
}  // namespace UKControllerPluginTest