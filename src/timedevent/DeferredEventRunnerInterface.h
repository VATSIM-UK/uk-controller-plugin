#pragma once
#include "pch/stdafx.h"

namespace UKControllerPlugin {
    namespace TimedEvent {

        /*
            Represents a deferred event that we want to run
            at some point in the future.
        */
        class DeferredEventRunnerInterface
        {
            public:
                virtual void Run(void) = 0;
        };
    }  // namespace TimedEvent
}  // namespace UKControllerPlugin
