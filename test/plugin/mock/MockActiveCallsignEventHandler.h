#pragma once
#include "controller/ActiveCallsignEventHandlerInterface.h"
#include "controller/ActiveCallsign.h"

namespace UKControllerPluginTest::Controller {

    class MockActiveCallsignEventHandler : public UKControllerPlugin::Controller::ActiveCallsignEventHandlerInterface
    {
        public:
        MOCK_METHOD(void, ActiveCallsignAdded, (const UKControllerPlugin::Controller::ActiveCallsign&), ());
        MOCK_METHOD(void, ActiveCallsignRemoved, (const UKControllerPlugin::Controller::ActiveCallsign&), ());
        MOCK_METHOD(void, CallsignsFlushed, (), ());
    };
} // namespace UKControllerPluginTest::Controller
