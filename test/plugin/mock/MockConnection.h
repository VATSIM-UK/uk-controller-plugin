#pragma once
#include "integration/Connection.h"

namespace UKControllerPluginTest::Integration {
    class MockConnection : public UKControllerPlugin::Integration::Connection
    {
        public:
        MOCK_METHOD(void, Send, (std::string), (override));
        MOCK_METHOD(std::queue<std::string>, Receive, (), (override));
        MOCK_METHOD(bool, Active, (), (const, override));
    };
} // namespace UKControllerPluginTest::Integration
