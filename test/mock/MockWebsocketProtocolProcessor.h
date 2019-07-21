#pragma once
#include "pch/pch.h"
#include "websocket/WebsocketProtocolProcessorInterface.h"

using UKControllerPlugin::Websocket::WebsocketProtocolProcessorInterface;

namespace UKControllerPluginTest {
    namespace Websocket {
        class MockWebsocketProtocolProcessor : public WebsocketProtocolProcessorInterface
        {
            public:
                MOCK_METHOD1(ProcessProtocolMessage, void(const UKControllerPlugin::Websocket::WebsocketMessage &));
                MOCK_CONST_METHOD0(GetEventSubscriptions, std::set<std::string>(void));
        };
    }  // namespace Websocket
}  // namespace UKControllerPluginTest
