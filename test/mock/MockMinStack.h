#pragma once
#include "minstack/MinStackManager.h"

namespace UKControllerPluginTest {
    namespace MinStack {
        class MockMinStack : public UKControllerPlugin::MinStack::MinStackManager
        {
        public:
            MockMinStack() : MinStackManager() {}
            MOCK_METHOD1(IsConcernedAirfield, bool(std::string));
            MOCK_METHOD2(NewMetar, void(std::string, std::string));
        };
    }  // namespace MinStack
}  // namespace UKControllerPluginTest
