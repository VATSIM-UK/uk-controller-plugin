#pragma once
#include "pch/pch.h"
#include "websocket/PusherActivityTimeoutEventHandler.h"
#include "websocket/WebsocketSubscription.h"
#include "mock/MockWebsocketConnection.h"

using ::testing::Test;
using UKControllerPlugin::Websocket::PusherActivityTimeoutEventHandler;
using UKControllerPlugin::Websocket::WebsocketSubscription;
using UKControllerPlugin::Websocket::WebsocketMessage;
using UKControllerPluginTest::Websocket::MockWebsocketConnection;
using testing::NiceMock;
using testing::Return;

namespace UKControllerPluginTest {
    namespace Websocket {

        class PusherActivityTimeoutEventHandlerTest : public Test
        {
            public:
                PusherActivityTimeoutEventHandlerTest()
                    : handler(mockWebsocket)
                {

                }

                NiceMock<MockWebsocketConnection> mockWebsocket;
                PusherActivityTimeoutEventHandler handler;
        };

        TEST_F(PusherActivityTimeoutEventHandlerTest, ItReturnsSubscriptions)
        {
            std::set<WebsocketSubscription> expected = {
                {
                    WebsocketSubscription::SUB_TYPE_EVENT,
                    "pusher:connection_established"
                },
                {
                    WebsocketSubscription::SUB_TYPE_EVENT,
                    "pusher:pong"
                }
            };

            EXPECT_EQ(expected, this->handler.GetSubscriptions());
        }

        TEST_F(PusherActivityTimeoutEventHandlerTest, ItSetsTheTimeoutIntervalFromServerIfLower)
        {
            EXPECT_CALL(this->mockWebsocket, SetIdleTimeout(std::chrono::seconds(40)))
                .Times(1);

            WebsocketMessage message{
                "pusher:connection_established",
                "none",
                nlohmann::json({{"activity_timeout", 20}}),
                true
            };
            this->handler.ProcessWebsocketMessage(message);
        }

        TEST_F(PusherActivityTimeoutEventHandlerTest, ItSetsTheTimeoutIntervalFromDefaultIfLower)
        {
            EXPECT_CALL(this->mockWebsocket, SetIdleTimeout(std::chrono::seconds(60)))
                .Times(1);

            WebsocketMessage message{
                "pusher:connection_established",
                "none",
                nlohmann::json({{"activity_timeout", 9999999}}),
                true
            };
            this->handler.ProcessWebsocketMessage(message);
        }
    }  // namespace Websocket
}  // namespace UKControllerPluginTest
