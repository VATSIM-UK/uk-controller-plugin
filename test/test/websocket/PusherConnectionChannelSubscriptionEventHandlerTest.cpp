#pragma once
#include "pch/pch.h"
#include "websocket/PusherConnectionChannelSubscriptionEventHandler.h"
#include "websocket/WebsocketSubscription.h"
#include "mock/MockWebsocketConnection.h"
#include "mock/MockWebsocketEventProcessor.h"
#include "mock/MockApiInterface.h"
#include "mock/MockTaskRunnerInterface.h"
#include "api/ApiException.h"

using ::testing::Test;
using UKControllerPlugin::Websocket::PusherConnectionChannelSubscriptionEventHandler;
using UKControllerPlugin::Websocket::WebsocketSubscription;
using UKControllerPlugin::Websocket::WebsocketMessage;
using UKControllerPlugin::Websocket::WebsocketEventProcessorCollection;
using UKControllerPluginTest::Websocket::MockWebsocketConnection;
using UKControllerPluginTest::Websocket::MockWebsocketEventProcessor;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPlugin::Api::ApiException;
using UKControllerPluginTest::TaskManager::MockTaskRunnerInterface;
using testing::NiceMock;
using testing::Return;
using testing::Throw;
using testing::_;

namespace UKControllerPluginTest {
    namespace Websocket {

        class PusherConnectionChannelSubscriptionEventHandlerTest : public Test
        {
            public:
                PusherConnectionChannelSubscriptionEventHandlerTest()
                    : handler(mockWebsocket, processors, mockApi, mockTaskRunner)
                {
                    this->mockProcessor.reset(new NiceMock<MockWebsocketEventProcessor>);
                }

                NiceMock<MockWebsocketConnection> mockWebsocket;
                std::shared_ptr<NiceMock<MockWebsocketEventProcessor>> mockProcessor;
                NiceMock<MockApiInterface> mockApi;
                NiceMock<MockTaskRunnerInterface> mockTaskRunner;
                WebsocketEventProcessorCollection processors;
                PusherConnectionChannelSubscriptionEventHandler handler;
        };
        
        TEST_F(PusherConnectionChannelSubscriptionEventHandlerTest, ItReturnsSubscriptions)
        {
            std::set<WebsocketSubscription> expected = {
                {
                    WebsocketSubscription::SUB_TYPE_EVENT,
                    "pusher:connection_established"
                }
            };

            EXPECT_EQ(expected, this->handler.GetSubscriptions());
        }

        TEST_F(PusherConnectionChannelSubscriptionEventHandlerTest, ItKnowsWhatChannelsArePrivate)
        {
            EXPECT_TRUE(this->handler.ChannelIsPrivate("private-minstack-levels"));
        }

        TEST_F(PusherConnectionChannelSubscriptionEventHandlerTest, ItKnowsWhatChannelsArentPrivate)
        {
            EXPECT_FALSE(this->handler.ChannelIsPrivate("privateminstack-levels"));
        }

        TEST_F(PusherConnectionChannelSubscriptionEventHandlerTest, ItSubscribesToPublicChannels)
        {
            std::set<WebsocketSubscription> subs = {
                {
                    WebsocketSubscription::SUB_TYPE_CHANNEL,
                    "minstack-updates"
                }
            };

            ON_CALL(*this->mockProcessor, GetSubscriptions())
                .WillByDefault(Return(subs));

            this->processors.AddProcessor(this->mockProcessor);

            WebsocketMessage message {
                "pusher:connection_established",
                "none",
                nlohmann::json({{"socket_id", "abc"}}),
                true
            };

            nlohmann::json expectedMessage;
            expectedMessage["event"] = "pusher:subscribe";
            expectedMessage["data"] = {
                {"channel", "minstack-updates"}
            };

            EXPECT_CALL(this->mockWebsocket, WriteMessage(expectedMessage.dump()))
                .Times(1);

            this->handler.ProcessWebsocketMessage(message);
        }

        TEST_F(PusherConnectionChannelSubscriptionEventHandlerTest, ItSubscribesToPrivateChannels)
        {
            std::set<WebsocketSubscription> subs = {
                {
                    WebsocketSubscription::SUB_TYPE_CHANNEL,
                    "private-minstack-updates"
                }
            };

            ON_CALL(*this->mockProcessor, GetSubscriptions())
                .WillByDefault(Return(subs));

            this->processors.AddProcessor(this->mockProcessor);

            WebsocketMessage message {
                "pusher:connection_established",
                "none",
                nlohmann::json({{"socket_id", "abc"}}),
                true
            };

            EXPECT_CALL(this->mockApi, AuthoriseWebsocketChannel("abc", "private-minstack-updates"))
                .Times(1)
                .WillOnce(Return("def"));
                
            nlohmann::json expectedMessage;
            expectedMessage["event"] = "pusher:subscribe";
            expectedMessage["data"] = {
                {"channel", "private-minstack-updates"},
                {"auth", "def"}
            };

            EXPECT_CALL(this->mockWebsocket, WriteMessage(expectedMessage.dump()))
                .Times(1);

            this->handler.ProcessWebsocketMessage(message);
        }

        TEST_F(PusherConnectionChannelSubscriptionEventHandlerTest, ItHandlesApiException)
        {
            std::set<WebsocketSubscription> subs = {
                {
                    WebsocketSubscription::SUB_TYPE_CHANNEL,
                    "private-minstack-updates"
                }
            };

            ON_CALL(*this->mockProcessor, GetSubscriptions())
                .WillByDefault(Return(subs));

            this->processors.AddProcessor(this->mockProcessor);

            WebsocketMessage message {
                "pusher:connection_established",
                "none",
                nlohmann::json({{"socket_id", "abc"}}),
                true
            };

            EXPECT_CALL(this->mockApi, AuthoriseWebsocketChannel("abc", "private-minstack-updates"))
                .Times(1)
                .WillOnce(Throw(ApiException("test")));

            EXPECT_CALL(this->mockWebsocket, WriteMessage(_))
                .Times(0);

            this->handler.ProcessWebsocketMessage(message);
        }

        TEST_F(PusherConnectionChannelSubscriptionEventHandlerTest, ItCachesChannelAuthCodesForReuse)
        {
            std::set<WebsocketSubscription> subs = {
                {
                    WebsocketSubscription::SUB_TYPE_CHANNEL,
                    "private-minstack-updates"
                }
            };

            ON_CALL(*this->mockProcessor, GetSubscriptions())
                .WillByDefault(Return(subs));

            this->processors.AddProcessor(this->mockProcessor);

            WebsocketMessage message{
                "pusher:connection_established",
                "none",
                nlohmann::json({{"socket_id", "abc"}}),
                true
            };

            EXPECT_CALL(this->mockApi, AuthoriseWebsocketChannel("abc", "private-minstack-updates"))
                .Times(1)
                .WillOnce(Return("def"));

            this->handler.ProcessWebsocketMessage(message);

            EXPECT_EQ("def", this->handler.GetCachedAuthCode("private-minstack-updates"));
        }
    }  // namespace Websocket
}  // namespace UKControllerPluginTest
