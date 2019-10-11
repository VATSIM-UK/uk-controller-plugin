#include "pch/pch.h"
#include "websocket/PusherWebsocketProtocolHandler.h"
#include "websocket/WebsocketEventProcessorCollection.h"
#include "mock/MockWebsocketEventProcessor.h"
#include "mock/MockWebsocketConnection.h"
#include "websocket/WebsocketSubscription.h"

using UKControllerPlugin::Websocket::PusherWebsocketProtocolHandler;
using UKControllerPlugin::Websocket::WebsocketEventProcessorCollection;
using UKControllerPluginTest::Websocket::MockWebsocketConnection;
using UKControllerPluginTest::Websocket::MockWebsocketEventProcessor;
using UKControllerPlugin::Websocket::WebsocketSubscription;
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

                    std::set<WebsocketSubscription> channelSubs = { subChannel1, subChannel2 };

                    ON_CALL(*this->mockEventProcessor, GetSubscriptions())
                        .WillByDefault(Return(channelSubs));

                    this->processorCollection.AddProcessor(this->mockEventProcessor);
                }

                NiceMock<MockWebsocketConnection> websocket;
                WebsocketEventProcessorCollection processorCollection;

                std::shared_ptr<NiceMock<MockWebsocketEventProcessor>> mockEventProcessor;
                WebsocketSubscription subChannel1 = { WebsocketSubscription::SUB_TYPE_CHANNEL, "channel1" };
                WebsocketSubscription subChannel2 = { WebsocketSubscription::SUB_TYPE_CHANNEL, "channel2" };

                PusherWebsocketProtocolHandler handler;
        };

        TEST_F(PusherWebsocketProtocolHandlerTest, ItDoesNothingOnNoMessage)
        {
            EXPECT_CALL(*this->mockEventProcessor, ProcessWebsocketMessage(_))
                .Times(0);

            ON_CALL(this->websocket, GetNextMessage)
                .WillByDefault(Return(this->websocket.noMessage));

            this->handler.TimedEventTrigger();
        }

        TEST_F(PusherWebsocketProtocolHandlerTest, ItDoesNothingOnInvalidMessage)
        {
            EXPECT_CALL(*this->mockEventProcessor, ProcessWebsocketMessage(_))
                .Times(0);

            EXPECT_CALL(this->websocket, GetNextMessage)
                .Times(2)
                .WillOnce(Return("{]"))
                .WillOnce(Return(this->websocket.noMessage));

            this->handler.TimedEventTrigger();
        }

        TEST_F(PusherWebsocketProtocolHandlerTest, ItHandlesMessages)
        {
            EXPECT_CALL(*this->mockEventProcessor, ProcessWebsocketMessage(_))
                .Times(1);

            nlohmann::json eventMessage;
            eventMessage["channel"] = "channel1";
            eventMessage["event"] = "test-event";

            EXPECT_CALL(this->websocket, GetNextMessage)
                .Times(2)
                .WillOnce(Return(eventMessage.dump()))
                .WillOnce(Return(this->websocket.noMessage));

            this->handler.TimedEventTrigger();
        }
    }  // namespace Websocket
}  // namespace UKControllerPluginTest
