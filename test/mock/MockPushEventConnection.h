#pragma once
#include "pch/pch.h"
#include "push/PushEventConnectionInterface.h"

namespace UKControllerPluginTest {
    namespace Push {
        class MockPushEventConnection : public UKControllerPlugin::Push::PushEventConnectionInterface
        {
            public:
                MOCK_METHOD1(WriteMessage, void(std::string));
                MOCK_METHOD0(GetNextMessage, std::string(void));
                MOCK_METHOD1(SetIdleTimeout, void(std::chrono::seconds timeout));
        };
    } // namespace Push
}  // namespace UKControllerPluginTest
