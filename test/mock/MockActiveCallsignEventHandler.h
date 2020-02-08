#pragma once
#include "pch/pch.h"
#include "controller/ActiveCallsignEventHandlerInterface.h"
#include "controller/ActiveCallsign.h"

namespace UKControllerPluginTest {
    namespace Controller {

        class MockActiveCallsignEventHandler
            : public UKControllerPlugin::Controller::ActiveCallsignEventHandlerInterface
        {
            public:
                MOCK_METHOD(
                    void,
                    ActiveCallsignAdded,
                    (const UKControllerPlugin::Controller::ActiveCallsign &, bool),
                    ()
                );
                MOCK_METHOD(
                    void,
                    ActiveCallsignRemoved,
                    (const UKControllerPlugin::Controller::ActiveCallsign &, bool),
                    ()
                );
                MOCK_METHOD(void, CallsignsFlushed, (), ());
                
        };
    }  // namespace Controller
}  // namespace UKControllerPluginTest
