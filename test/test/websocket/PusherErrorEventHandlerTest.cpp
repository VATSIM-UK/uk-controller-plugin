#pragma once
#include "pch/pch.h"
#include "websocket/PusherErrorEventHandler.h"
#include "websocket/WebsocketSubscription.h"

using ::testing::Test;
using UKControllerPlugin::Websocket::PusherErrorEventHandler;
using UKControllerPlugin::Websocket::WebsocketSubscription;

namespace UKControllerPluginTest {
    namespace Websocket {

        class PusherErrorEventHandlerTest : public Test
        {
            public:

                PusherErrorEventHandler handler;
        };

        TEST_F(PusherErrorEventHandlerTest, ItReturnsSubscriptions)
        {
            std::set<WebsocketSubscription> expected = {
                {
                    WebsocketSubscription::SUB_TYPE_EVENT,
                    "pusher:error"
                }
            };

            EXPECT_EQ(expected, this->handler.GetSubscriptions());
        }

        TEST_F(PusherErrorEventHandlerTest, ItProcessesErrors)
        {
            EXPECT_NO_THROW(this->handler.ProcessWebsocketMessage({ "test", "test", {} }));
        }
    }  // namespace Websocket
}  // namespace UKControllerPluginTest
