#include "pch/pch.h"
#include "websocket/PusherWebsocketProtocolHandler.h"
#include "websocket/WebsocketEventProcessorCollection.h"
#include "mock/MockWebsocketEventProcessor.h"
#include "mock/MockWebsocketProtocolProcessor.h"
#include "mock/MockWebsocketConnection.h"

using UKControllerPlugin::Websocket::PusherWebsocketProtocolHandler;
using UKControllerPlugin::Websocket::WebsocketEventProcessorCollection;
using UKControllerPluginTest::Websocket::MockWebsocketConnection;
using UKControllerPluginTest::Websocket::MockWebsocketEventProcessor;
using UKControllerPluginTest::Websocket::MockWebsocketProtocolProcessor;
using ::testing::Test;
using ::testing::Return;
using ::testing::NiceMock;
using ::testing::_;

namespace UKControllerPluginTest {
    namespace Websocket {

        class PusherWebsocketProtocolHandlerTest : public Test
        {
            public:

                PusherWebsocketProtocolHandlerTest()
                    : handler(this->websocket, this->processorCollection)
                {
                    mockEventProcessor.reset(new NiceMock<MockWebsocketEventProcessor>);
                    mockProtocolProcessor.reset(new NiceMock<MockWebsocketProtocolProcessor>);

                    ON_CALL(*this->mockEventProcessor, GetSubscriptions())
                        .WillByDefault(Return(this->channelSubs));

                    ON_CALL(*this->mockProtocolProcessor, GetEventSubscriptions())
                        .WillByDefault(Return(this->protocolSubs));

                    this->processorCollection.AddProcessor(this->mockEventProcessor);
                    this->processorCollection.AddProtocolProcessor(this->mockProtocolProcessor);
                }

                NiceMock<MockWebsocketConnection> websocket;
                WebsocketEventProcessorCollection processorCollection;

                std::shared_ptr<NiceMock<MockWebsocketEventProcessor>> mockEventProcessor;
                std::set<std::string> channelSubs = { "channel1", "channel2" };

                std::shared_ptr<NiceMock<MockWebsocketProtocolProcessor>> mockProtocolProcessor;
                std::set<std::string> protocolSubs = { "pusher:test" };

                PusherWebsocketProtocolHandler handler;
        };

        TEST_F(PusherWebsocketProtocolHandlerTest, ItDoesNothingOnNoMessage)
        {
            EXPECT_CALL(*this->mockEventProcessor, ProcessWebsocketMessage(_))
                .Times(0);

            EXPECT_CALL(*this->mockProtocolProcessor, ProcessProtocolMessage(_))
                .Times(0);

            ON_CALL(this->websocket, GetNextMessage)
                .WillByDefault(Return(this->websocket.noMessage));

            this->handler.TimedEventTrigger();
        }

        TEST_F(PusherWebsocketProtocolHandlerTest, ItDoesNothingOnInvalidMessage)
        {
            EXPECT_CALL(*this->mockEventProcessor, ProcessWebsocketMessage(_))
                .Times(0);

            EXPECT_CALL(*this->mockProtocolProcessor, ProcessProtocolMessage(_))
                .Times(0);

            EXPECT_CALL(this->websocket, GetNextMessage)
                .Times(2)
                .WillOnce(Return("{]"))
                .WillOnce(Return(this->websocket.noMessage));

            this->handler.TimedEventTrigger();
        }

        TEST_F(PusherWebsocketProtocolHandlerTest, ItHandlesChannelMessages)
        {
            EXPECT_CALL(*this->mockEventProcessor, ProcessWebsocketMessage(_))
                .Times(1);

            EXPECT_CALL(*this->mockProtocolProcessor, ProcessProtocolMessage(_))
                .Times(0);

            nlohmann::json eventMessage;
            eventMessage["channel"] = "channel1";
            eventMessage["event"] = "test-event";

            EXPECT_CALL(this->websocket, GetNextMessage)
                .Times(2)
                .WillOnce(Return(eventMessage.dump()))
                .WillOnce(Return(this->websocket.noMessage));

            this->handler.TimedEventTrigger();
        }

        TEST_F(PusherWebsocketProtocolHandlerTest, ItHandlesProtocolMessages)
        {
            EXPECT_CALL(*this->mockEventProcessor, ProcessWebsocketMessage(_))
                .Times(0);

            EXPECT_CALL(*this->mockProtocolProcessor, ProcessProtocolMessage(_))
                .Times(1);

            nlohmann::json eventMessage;
            eventMessage["event"] = "pusher:test";

            EXPECT_CALL(this->websocket, GetNextMessage)
                .Times(2)
                .WillOnce(Return(eventMessage.dump()))
                .WillOnce(Return(this->websocket.noMessage));

            this->handler.TimedEventTrigger();
        }
    }  // namespace Websocket
}  // namespace UKControllerPluginTest
