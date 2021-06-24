#include "pch/pch.h"
#include "push/PushEventProxyConnection.h"

using ::testing::Test;
using UKControllerPlugin::Push::PushEventProxyConnection;

namespace UKControllerPluginTest {
    namespace Push {

        class PushEventProxyConnectionTest : public Test
        {
            public:
                PushEventProxyConnection connection;
        };

        TEST_F(PushEventProxyConnectionTest, ItReturnsNoMessageIfNothingToProcess)
        {
            EXPECT_EQ("", connection.GetNextMessage());
        }

        TEST_F(PushEventProxyConnectionTest, ItReturnsAllMessagesOnQueue)
        {
            connection.AddMessageToQueue("a");
            connection.AddMessageToQueue("b");
            connection.AddMessageToQueue("c");
            EXPECT_EQ("a", connection.GetNextMessage());
            EXPECT_EQ("b", connection.GetNextMessage());
            EXPECT_EQ("c", connection.GetNextMessage());
            EXPECT_EQ("", connection.GetNextMessage());
        }

        TEST_F(PushEventProxyConnectionTest, ItLoadsHiddenWindow)
        {
            EXPECT_NE(
                nullptr,
                FindWindowA("UKControllerPluginPushEventProxyClass", "UKControllerPluginPushEventProxy")
            );
        }
    } // namespace Push
}  // namespace UKControllerPluginTest
