#pragma once
#include "pch/pch.h"
#include "websocket/WebsocketConnection.h"

namespace UKControllerPluginTest {
    namespace Websocket {
        class MockWebsocketConnection : public UKControllerPlugin::Websocket::WebsocketConnectionInterface
        {
            public:
                MOCK_METHOD0(ForceDisconnect, void(void));
                MOCK_METHOD1(WriteMessage, void(std::string));
                MOCK_METHOD1(SetIdleTimeout, void(std::chrono::seconds));
                MOCK_METHOD0(GetNextMessage, std::string(void));
                MOCK_CONST_METHOD0(GetTimeSinceLastActivity, std::chrono::seconds(void));
        };
    }  // namespace Websocket
}  // namespace UKControllerPluginTest
