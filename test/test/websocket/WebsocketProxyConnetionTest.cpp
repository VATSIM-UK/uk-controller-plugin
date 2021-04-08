#include "pch/pch.h"
#include "websocket/WebsocketProxyConnection.h"

using ::testing::Test;
using UKControllerPlugin::Websocket::WebsocketProxyConnection;

namespace UKControllerPluginTest {
    namespace Websocket {

        class WebsocketProxyConnectionTest : public Test
        {
            public:
                WebsocketProxyConnection connection;
        };

        TEST_F(WebsocketProxyConnectionTest, ItReturnsNoMessageIfNothingToProcess)
        {
            EXPECT_EQ("", connection.GetNextMessage());
        }

        TEST_F(WebsocketProxyConnectionTest, ItReturnsAllMessagesOnQueue)
        {
            connection.AddMessageToQueue("a");
            connection.AddMessageToQueue("b");
            connection.AddMessageToQueue("c");
            EXPECT_EQ("a", connection.GetNextMessage());
            EXPECT_EQ("b", connection.GetNextMessage());
            EXPECT_EQ("c", connection.GetNextMessage());
            EXPECT_EQ("", connection.GetNextMessage());
        }

        TEST_F(WebsocketProxyConnectionTest, ItLoadsHiddenWindow)
        {
            EXPECT_NE(
                nullptr,
                FindWindowA("UKControllerPluginWebsocketProxyClass", "UKControllerPluginWebsocketProxy")
            );
        }
    } // namespace Websocket
}  // namespace UKControllerPluginTest
