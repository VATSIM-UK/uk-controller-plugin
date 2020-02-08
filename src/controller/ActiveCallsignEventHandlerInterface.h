#pragma once
#include "controller/ActiveCallsign.h"

namespace UKControllerPlugin {
    namespace Controller {

        /*
            An interface to be implemented by classes that want to know
            when Active Callsigns come and go.
        */
        class ActiveCallsignEventHandlerInterface
        {
            public:
                virtual void ActiveCallsignAdded(
                    const UKControllerPlugin::Controller::ActiveCallsign& callsign,
                    bool userCallsign
                ) = 0;
                virtual void ActiveCallsignRemoved(
                    const UKControllerPlugin::Controller::ActiveCallsign& callsign,
                    bool userCallsign
                ) = 0;
                virtual void CallsignsFlushed(void) = 0;
        };
    }  // namespace Controller
}  // namespace UKControllerPlugin
