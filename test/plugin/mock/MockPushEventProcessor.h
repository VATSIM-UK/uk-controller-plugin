#pragma once
#include "push/PushEventProcessorInterface.h"
#include "push/PushEventSubscription.h"

namespace UKControllerPluginTest {
    namespace Push {
        class MockPushEventProcessor : public UKControllerPlugin::Push::PushEventProcessorInterface
        {
            public:
            MockPushEventProcessor();
            virtual ~MockPushEventProcessor();
            MOCK_METHOD1(ProcessPushEvent, void(const UKControllerPlugin::Push::PushEvent&));
            MOCK_CONST_METHOD0(
                GetPushEventSubscriptions, std::set<UKControllerPlugin::Push::PushEventSubscription>(void));
            MOCK_METHOD(void, PluginEventsSynced, (), (override));
        };
    } // namespace Push
} // namespace UKControllerPluginTest
