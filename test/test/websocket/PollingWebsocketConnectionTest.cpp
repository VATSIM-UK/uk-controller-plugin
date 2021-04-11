#include "pch/pch.h"
#include "websocket/PollingWebsocketConnection.h"

using ::testing::Test;
using UKControllerPlugin::Websocket::PollingWebsocketConnection;

namespace UKControllerPluginTest {
    namespace Websocket {

        class PollingWebsocketConnectionTest : public Test
        {

        };

        TEST_F(PollingWebsocketConnectionTest, ItDoesSomething)
        {
            EXPECT_TRUE(true);
        }
    }  // namespace Websocket
}  // namespace UKControllerPluginTest
