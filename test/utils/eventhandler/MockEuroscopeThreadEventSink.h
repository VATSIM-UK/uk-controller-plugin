#pragma once
#include "eventhandler/EuroscopeThreadEventSink.h"

namespace UKControllerPluginUtilsTest::EventHandler {
    class MockEuroscopeThreadEventSink : public UKControllerPluginUtils::EventHandler::EuroscopeThreadEventSink
    {
        public:
        MOCK_METHOD(void, OnEvent, (const std::function<void()>&), (override));
    };
} // namespace UKControllerPluginUtilsTest::EventHandler
