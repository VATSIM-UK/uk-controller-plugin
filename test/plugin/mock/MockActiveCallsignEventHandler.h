#pragma once
#include "controller/ActiveCallsign.h"
#include "controller/ActiveCallsignEventHandlerInterface.h"

namespace UKControllerPluginTest::Controller {

    class MockActiveCallsignEventHandler : public UKControllerPlugin::Controller::ActiveCallsignEventHandlerInterface
    {
        public:
        MockActiveCallsignEventHandler();
        virtual ~MockActiveCallsignEventHandler();
        MOCK_METHOD(void, ActiveCallsignAdded, (const UKControllerPlugin::Controller::ActiveCallsign&), ());
        MOCK_METHOD(void, ActiveCallsignRemoved, (const UKControllerPlugin::Controller::ActiveCallsign&), ());
        MOCK_METHOD(void, CallsignsFlushed, (), ());
    };
} // namespace UKControllerPluginTest::Controller
