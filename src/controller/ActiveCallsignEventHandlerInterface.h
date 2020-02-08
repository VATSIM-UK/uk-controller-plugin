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
                virtual void ActiveCallsignAdded(const ActiveCallsign& callsign) = 0;
                virtual void ActiveCallsignRemoved(const ActiveCallsign& callsign) = 0;
                virtual void CallsignsFlushed(void) = 0;
        };
    }  // namespace Controller
}  // namespace UKControllerPlugin
