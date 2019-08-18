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

        TEST_F(PusherActivityTimeoutEventHandlerTest, ItSetsThePingIntervalFromServerIfLower)
        {
            WebsocketMessage message{
                "pusher:connection_established",
                "none",
                nlohmann::json({{"activity_timeout", 30}}),
                true
            };
            this->handler.ProcessWebsocketMessage(message);
            EXPECT_EQ(std::chrono::seconds(30), this->handler.GetPingInterval());
        }

        TEST_F(PusherActivityTimeoutEventHandlerTest, ItSetsThePingIntervalFromDefaultIfLower)
        {
            WebsocketMessage message{
                "pusher:connection_established",
                "none",
                nlohmann::json({{"activity_timeout", 9999999}}),
                true
            };
            this->handler.ProcessWebsocketMessage(message);
            EXPECT_EQ(this->handler.defaultPingInterval, this->handler.GetPingInterval());
        }

        TEST_F(PusherActivityTimeoutEventHandlerTest, ItSendsAPingIfTimeHasPassedSinceActivity)
        {
            WebsocketMessage message{
                "pusher:connection_established",
                "none",
                nlohmann::json({{"activity_timeout", 1}}),
                true
            };
            this->handler.ProcessWebsocketMessage(message);

            ON_CALL(this->mockWebsocket, GetTimeSinceLastActivity())
                .WillByDefault(Return(std::chrono::seconds(5)));

            nlohmann::json expectedOutboundMessage = {
                {"event", "pusher:ping"}
            };
            EXPECT_CALL(this->mockWebsocket, WriteMessage(expectedOutboundMessage.dump()))
                .Times(1);

            this->handler.TimedEventTrigger();
        }

        TEST_F(PusherActivityTimeoutEventHandlerTest, ItSetsPongTimeoutOnPing)
        {
            WebsocketMessage message{
                "pusher:connection_established",
                "none",
                nlohmann::json({{"activity_timeout", 1}}),
                true
            };
            this->handler.ProcessWebsocketMessage(message);

            ON_CALL(this->mockWebsocket, GetTimeSinceLastActivity())
                .WillByDefault(Return(std::chrono::seconds(5)));

            this->handler.TimedEventTrigger();

            EXPECT_NE((std::chrono::system_clock::time_point::min)(), this->handler.GetPongTimeout());
        }

        TEST_F(PusherActivityTimeoutEventHandlerTest, PongResetsTheTimeout)
        {
            WebsocketMessage message{
                "pusher:connection_established",
                "none",
                nlohmann::json({{"activity_timeout", 1}}),
                true
            };
            this->handler.ProcessWebsocketMessage(message);

            ON_CALL(this->mockWebsocket, GetTimeSinceLastActivity())
                .WillByDefault(Return(std::chrono::seconds(5)));

            this->handler.TimedEventTrigger();

            WebsocketMessage message2{
                "pusher:pong",
                "none",
                nlohmann::json(),
                true
            };

            this->handler.ProcessWebsocketMessage(message2);

            EXPECT_EQ((std::chrono::system_clock::time_point::min)(), this->handler.GetPongTimeout());
        }

        TEST_F(PusherActivityTimeoutEventHandlerTest, PongTimeoutForcesDisconnect)
        {
            WebsocketMessage message{
                "pusher:connection_established",
                "none",
                nlohmann::json({{"activity_timeout", 1}}),
                true
            };
            this->handler.ProcessWebsocketMessage(message);

            EXPECT_CALL(this->mockWebsocket, GetTimeSinceLastActivity())
                .Times(2)
                .WillOnce(Return(std::chrono::seconds(5)))
                .WillOnce(Return(std::chrono::seconds(0)));

            this->handler.TimedEventTrigger();


            EXPECT_CALL(this->mockWebsocket, ForceDisconnect())
                .Times(1);

            // Force pong timeout
            this->handler.SetPongTimeout(std::chrono::system_clock::now() + std::chrono::seconds(15));
            this->handler.TimedEventTrigger();
        }
    }  // namespace Websocket
}  // namespace UKControllerPluginTest
