#include "pch/pch.h"
#include "websocket/PusherPingEventHandler.h"
#include "websocket/WebsocketSubscription.h"
#include "mock/MockWebsocketConnection.h"

using UKControllerPlugin::Websocket::PusherPingEventHandler;
using UKControllerPlugin::Websocket::WebsocketSubscription;
using testing::Test;
using testing::NiceMock;

namespace UKControllerPluginTest {
    namespace Websocket {

        class PusherPingEventHandlerTest : public Test
        {
            public:

                PusherPingEventHandlerTest()
                    : handler(mockWebsocket)
                {

                }
                
                NiceMock<MockWebsocketConnection> mockWebsocket;
                PusherPingEventHandler handler;
        };

        TEST_F(PusherPingEventHandlerTest, ItReturnsSubscriptions)
        {
            std::set<WebsocketSubscription> expected = {
                {
                    WebsocketSubscription::SUB_TYPE_EVENT,
                    "pusher:ping"
                }
            };

            EXPECT_EQ(expected, this->handler.GetSubscriptions());
        }

        TEST_F(PusherPingEventHandlerTest, ItRespondsToPingWithPong)
        {
            nlohmann::json expectedJson;
            expectedJson["event"] = "pusher:pong";

            EXPECT_CALL(this->mockWebsocket, WriteMessage(expectedJson.dump()))
                .Times(1);

            this->handler.ProcessWebsocketMessage({});
        }
    }  // namespace Websocket
}  // namespace UKControllerPluginTest
