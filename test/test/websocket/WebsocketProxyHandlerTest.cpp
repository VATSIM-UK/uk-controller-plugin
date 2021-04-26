#include "pch/pch.h"
#include "websocket/WebsocketProxyHandler.h"
#include "websocket/WebsocketSubscription.h"

using UKControllerPlugin::Websocket::WebsocketProxyHandler;
using UKControllerPlugin::Websocket::WebsocketSubscription;
using testing::Test;
using testing::NiceMock;

namespace UKControllerPluginTest {
    namespace Websocket {

        class WebsocketProxyHandlerTest : public Test
        {
            public:

                WebsocketProxyHandler handler;
        };

        TEST_F(WebsocketProxyHandlerTest, ItReturnsSubscriptions)
        {
            std::set<WebsocketSubscription> expected = {
                {
                    WebsocketSubscription::SUB_TYPE_ALL
                }
            };

            EXPECT_EQ(expected, this->handler.GetSubscriptions());
        }
    } // namespace Websocket
}  // namespace UKControllerPluginTest
