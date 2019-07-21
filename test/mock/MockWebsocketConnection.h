#pragma once
#include "pch/pch.h"
#include "websocket/WebsocketConnection.h"

namespace UKControllerPluginTest {
    namespace Websocket {
        class MockWebsocketConnection : public UKControllerPlugin::Websocket::WebsocketConnectionInterface
        {
            public:
                MOCK_METHOD1(WriteMessage, void(std::string));
                MOCK_METHOD0(GetNextMessage, std::string(void));
        };
    }  // namespace Websocket
}  // namespace UKControllerPluginTest
