#pragma once
#include "pch/pch.h"
#include "websocket/WebsocketConnectionInterface.h"

namespace UKControllerPluginTest {
    namespace Websocket {
        class MockWebsocketConnection : public UKControllerPlugin::Websocket::WebsocketConnectionInterface
        {
            public:
                MOCK_METHOD1(WriteMessage, void(std::string));
                MOCK_METHOD0(GetNextMessage, std::string(void));
                MOCK_METHOD1(SetIdleTimeout, void(std::chrono::seconds timeout));
        };
    }  // namespace Websocket
}  // namespace UKControllerPluginTest
