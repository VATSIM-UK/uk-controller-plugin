#pragma once
#include "timedevent/DeferredEventRunnerInterface.h"

namespace UKControllerPluginTest {
    namespace TimedEvent {

        class MockDeferredEventRunner : public UKControllerPlugin::TimedEvent::DeferredEventRunnerInterface
        {
            public:
                explicit MockDeferredEventRunner(int & timesRun)
                    : timesRun(timesRun)
                {

                }

                void Run(void) override
                {
                    timesRun++;
                }

                int TimesRun(void) const
                {
                    return this->timesRun;
                }


            protected:

                // How many times the deferred event has run
                int & timesRun;
        };
    }  // namespace TimedEvent
}  // namespace UKControllerPluginTest
