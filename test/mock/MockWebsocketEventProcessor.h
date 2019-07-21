#pragma once
#include "pch/pch.h"
#include "websocket/WebsocketEventProcessorInterface.h"

namespace UKControllerPluginTest {
    namespace Websocket {
        class MockWebsocketEventProcessor : public UKControllerPlugin::Websocket::WebsocketEventProcessorInterface
        {
            public:
                MOCK_METHOD1(ProcessWebsocketMessage, void(const UKControllerPlugin::Websocket::WebsocketMessage &));
                MOCK_CONST_METHOD0(GetSubscriptions, std::set<std::string>(void));
        };
    }  // namespace Websocket
}  // namespace UKControllerPluginTest
