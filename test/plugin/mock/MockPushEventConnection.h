#pragma once
#include "push/PushEventConnectionInterface.h"

namespace UKControllerPluginTest {
    namespace Push {
        class MockPushEventConnection : public UKControllerPlugin::Push::PushEventConnectionInterface
        {
            public:
            MockPushEventConnection();
            virtual ~MockPushEventConnection();
            MOCK_METHOD1(WriteMessage, void(std::string));
            MOCK_METHOD0(GetNextMessage, std::string(void));
        };
    } // namespace Push
} // namespace UKControllerPluginTest
